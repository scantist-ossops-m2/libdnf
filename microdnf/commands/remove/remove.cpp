/*
Copyright (C) 2020-2021 Red Hat, Inc.

This file is part of microdnf: https://github.com/rpm-software-management/libdnf/

Microdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Microdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with microdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "remove.hpp"

#include "../../context.hpp"

#include <libdnf/base/goal.hpp>
#include <libdnf/conf/option_string.hpp>
#include <libdnf/rpm/package.hpp>
#include <libdnf/rpm/package_set.hpp>
#include <libdnf/rpm/solv_query.hpp>
#include <libdnf/rpm/transaction.hpp>

#include <iostream>

#include "libdnf-cli/output/transaction_table.hpp"

namespace microdnf {

using namespace libdnf::cli;

void CmdRemove::set_argument_parser(Context & ctx) {
    patterns_to_remove_options = ctx.arg_parser.add_new_values();
    auto keys = ctx.arg_parser.add_new_positional_arg(
        "keys_to_match",
        ArgumentParser::PositionalArg::UNLIMITED,
        ctx.arg_parser.add_init_value(std::unique_ptr<libdnf::Option>(new libdnf::OptionString(nullptr))),
        patterns_to_remove_options);
    keys->set_short_description("List of keys to match");

    auto remove = ctx.arg_parser.add_new_command("remove");
    remove->set_short_description("remove a package or packages from your system");
    remove->set_description("");
    remove->set_named_args_help_header("Optional arguments:");
    remove->set_positional_args_help_header("Positional arguments:");
    remove->set_parse_hook_func([this, &ctx](
                                    [[maybe_unused]] ArgumentParser::Argument * arg,
                                    [[maybe_unused]] const char * option,
                                    [[maybe_unused]] int argc,
                                    [[maybe_unused]] const char * const argv[]) {
        ctx.select_command(this);
        return true;
    });

    remove->register_positional_arg(keys);

    ctx.arg_parser.get_root_command()->register_command(remove);
}

void CmdRemove::configure([[maybe_unused]] Context & ctx) {}

void CmdRemove::run(Context & ctx) {
    auto & solv_sack = *ctx.base.get_rpm_solv_sack();

    // To search in the system repository (installed packages)
    // Creates system repository in the repo_sack and loads it into rpm::SolvSack.
    solv_sack.create_system_repo(false);

    libdnf::Goal goal(&ctx.base);
    for (auto & pattern : *patterns_to_remove_options) {
        auto option = dynamic_cast<libdnf::OptionString *>(pattern.get());
        goal.add_rpm_remove(option->get_value());
    }
    if (goal.resolve(true) != libdnf::GoalProblem::NO_PROBLEM) {
        std::cout << goal.get_formated_all_problems() << std::endl;
        return;
    }

    if (!libdnf::cli::output::print_transaction_table(goal)) {
        return;
    }

    if (!userconfirm(ctx.base.get_config())) {
        std::cout << "Operation aborted." << std::endl;
        return;
    }

    libdnf::rpm::Transaction rpm_transaction(ctx.base);
    auto db_transaction = new_db_transaction(ctx);
    std::vector<std::unique_ptr<RpmTransactionItem>> transaction_items;

    fill_transactions(goal, db_transaction, rpm_transaction, transaction_items);

    auto time = std::chrono::system_clock::now().time_since_epoch();
    db_transaction->set_dt_start(std::chrono::duration_cast<std::chrono::seconds>(time).count());
    db_transaction->start();

    run_transaction(rpm_transaction);

    time = std::chrono::system_clock::now().time_since_epoch();
    db_transaction->set_dt_end(std::chrono::duration_cast<std::chrono::seconds>(time).count());
    db_transaction->finish(libdnf::transaction::TransactionState::DONE);
}

}  // namespace microdnf