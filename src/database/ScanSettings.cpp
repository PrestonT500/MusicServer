/*
 * Copyright (C) 2018 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScanSettings.hpp"

#include <Wt/Dbo/WtSqlTraits.h>

#include "utils/Logger.hpp"
#include "utils/Utils.hpp"

#include "Cluster.hpp"
#include "Session.hpp"

namespace {

const std::set<std::string> defaultClusterTypeNames =
{
	"GENRE",
	"ALBUMGROUPING",
	"MOOD",
	"ALBUMMOOD",
};

}

namespace Database {

void
ScanSettings::init(Session& session)
{
	session.checkUniqueLocked();

	pointer settings {get(session)};
	if (settings)
		return;

	settings = session.getDboSession().add(std::make_unique<ScanSettings>());
	settings.modify()->setClusterTypes(session, defaultClusterTypeNames );
}

ScanSettings::pointer
ScanSettings::get(Session& session)
{
	session.checkSharedLocked();

	return session.getDboSession().find<ScanSettings>();
}

std::set<boost::filesystem::path>
ScanSettings::getAudioFileExtensions() const
{
	auto extensions = splitString(_audioFileExtensions, " ");
	return std::set<boost::filesystem::path>(std::cbegin(extensions), std::cend(extensions));
}

std::vector<ClusterType::pointer>
ScanSettings::getClusterTypes() const
{
	return std::vector<ClusterType::pointer>(std::cbegin(_clusterTypes), std::cend(_clusterTypes));
}

void
ScanSettings::setMediaDirectory(boost::filesystem::path p)
{
	_mediaDirectory = stringTrimEnd(p.string(), "/\\");
}

template <typename It>
std::set<std::string> getNames(It begin, It end)
{
	std::set<std::string> names;
	std::transform(begin, end, std::inserter(names, std::begin(names)),
		[](const ClusterType::pointer& clusterType)
		{
			return clusterType->getName();
		});

	return names;
}

void
ScanSettings::setClusterTypes(Session& session, const std::set<std::string>& clusterTypeNames)
{
	session.checkUniqueLocked();

	bool needRescan {};

	// Create any missing cluster type
	for (const std::string& clusterTypeName : clusterTypeNames)
	{
		auto clusterType {ClusterType::getByName(session, clusterTypeName)};
		if (!clusterType)
		{
			LMS_LOG(DB, INFO) << "Creating cluster type " << clusterTypeName;
			clusterType = ClusterType::create(session, clusterTypeName);
			_clusterTypes.insert(clusterType);

			needRescan = true;
		}
	}

	// Delete no longer existing cluster types
	for (ClusterType::pointer& clusterType : _clusterTypes)
	{
		if (std::none_of(clusterTypeNames.begin(), clusterTypeNames.end(),
			[clusterType](const std::string& name) { return name == clusterType->getName(); }))
		{
			LMS_LOG(DB, INFO) << "Deleting cluster type " << clusterType->getName();
			clusterType.remove();
		}
	}

	if (needRescan)
		_scanVersion += 1;
}

} // namespace Database

