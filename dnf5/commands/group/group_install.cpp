/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "group_install.hpp"

#include <libdnf/comps/comps.hpp>
#include <libdnf/comps/group/group.hpp>
#include <libdnf/comps/group/query.hpp>

#include <iostream>

namespace dnf5 {

using namespace libdnf::cli;

void GroupInstallCommand::set_argument_parser() {
    auto & cmd = *get_argument_parser_command();
    cmd.set_short_description("Install comp groups, including their packages");

    with_optional = std::make_unique<GroupWithOptionalOption>(*this);
    group_specs = std::make_unique<GroupSpecArguments>(*this, ArgumentParser::PositionalArg::AT_LEAST_ONE);
}

void GroupInstallCommand::configure() {
    auto & context = get_context();
    context.set_load_system_repo(true);
    context.set_load_available_repos(Context::LoadAvailableRepos::ENABLED);
    context.set_available_repos_load_flags(libdnf::repo::Repo::LoadFlags::COMPS);
}

void GroupInstallCommand::run() {
    auto goal = get_context().get_goal();

    libdnf::GroupJobSettings settings;
    if (with_optional->get_value()) {
        settings.package_types |= libdnf::comps::PackageType::OPTIONAL;
    }
    //settings.strict = libdnf::GoalSetting::SET_FALSE;
    for (const auto & spec : group_specs->get_value()) {
        goal->add_group_install(spec, settings);
    }
}

}  // namespace dnf5
