/*
 * Copyright (C) 2014 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef HY_ADVISORYPKG_H
#define HY_ADVISORYPKG_H

#include <glib.h>

G_BEGIN_DECLS

/* hawkey */
#include "hy-types.h"

enum _hy_advisorypkg_param_e {
    HY_ADVISORYPKG_NAME,
    HY_ADVISORYPKG_EVR,
    HY_ADVISORYPKG_ARCH,
    HY_ADVISORYPKG_FILENAME
};

void hy_advisorypkg_free(HyAdvisoryPkg advisorypkg);
const char *hy_advisorypkg_get_string(HyAdvisoryPkg advisorypkg, int which);

void hy_advisorypkglist_free(HyAdvisoryPkgList pkglist);
int hy_advisorypkglist_count(HyAdvisoryPkgList pkglist);
HyAdvisoryPkg hy_advisorypkglist_get_clone(HyAdvisoryPkgList pkglist, int index);

G_END_DECLS

#endif /* HY_ADVISORYPKG_H */
