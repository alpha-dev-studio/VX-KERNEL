#ifndef SECURITY_ACLS_H
#define SECURITY_ACLS_H

#include <stdint.h>
#include <stdbool.h>

struct security_acl {
    uint32_t owner_uid;
    uint32_t group_gid;
    uint32_t permissions; // Read, Write, Execute bitmasks
};

bool security_check_permission(struct security_acl *acl, uint32_t accessor_uid, uint32_t requested_access);

#endif
