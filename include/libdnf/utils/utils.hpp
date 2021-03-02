/*
Copyright (C) 2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBDNF_UTILS_UTILS_HPP
#define LIBDNF_UTILS_UTILS_HPP

namespace libdnf {

/// Compares content of two files.
/// Returns "true" if files have the same content.
/// If content differs or error occurred (file doesn't exist, not readable, ...) returns "false".
bool have_files_same_content_noexcept(const char * file_path1, const char * file_path2) noexcept;

enum class ProblemRules {
        RULE_DISTUPGRADE=1,
        RULE_INFARCH, RULE_UPDATE,
        RULE_JOB, RULE_JOB_UNSUPPORTED,
        RULE_JOB_NOTHING_PROVIDES_DEP,
        RULE_JOB_UNKNOWN_PACKAGE,
        RULE_JOB_PROVIDED_BY_SYSTEM,
        RULE_PKG,
        RULE_BEST_1,
        RULE_BEST_2,
        RULE_PKG_NOT_INSTALLABLE_1,
        RULE_PKG_NOT_INSTALLABLE_2,
        RULE_PKG_NOT_INSTALLABLE_3,
        RULE_PKG_NOT_INSTALLABLE_4,
        RULE_PKG_NOTHING_PROVIDES_DEP,
        RULE_PKG_SAME_NAME,
        RULE_PKG_CONFLICTS,
        RULE_PKG_OBSOLETES,
        RULE_PKG_INSTALLED_OBSOLETES,
        RULE_PKG_IMPLICIT_OBSOLETES,
        RULE_PKG_REQUIRES,
        RULE_PKG_SELF_CONFLICT,
        RULE_YUMOBS,
        RULE_UNKNOWN
};

}  // namespace libdnf

#endif