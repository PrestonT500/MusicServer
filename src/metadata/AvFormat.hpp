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

#pragma once

#include <map>
#include <string>

#include "MetaData.hpp"

namespace MetaData
{

// Parse that makes use of AvFormat
class AvFormat : public Parser
{
	public:

		AvFormat(const std::map<std::string, std::string>& clusterMap
				= {
				{"GENRE", "Genre" },
				{"ALBUMGROUPING", "Group" }
				});


		boost::optional<Items> parse(const boost::filesystem::path& p);

	private:

		std::map<std::string,std::string> _clusterMap;
};

} // namespace MetaData

