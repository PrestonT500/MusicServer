/*
 * Copyright (C) 2023 Emeric Poupon
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

#include "database/UserId.hpp"
#include "utils/ILogger.hpp"
#include "utils/UUID.hpp"

#define LOG(sev, message)	LMS_LOG(FEEDBACK, sev, "[listenbrainz] " << message)

namespace Database
{
    class Session;
}

namespace Feedback::ListenBrainz::Utils
{
    std::optional<UUID>	getListenBrainzToken(Database::Session& session, Database::UserId userId);
    std::string parseValidateToken(std::string_view msgBody);
}
