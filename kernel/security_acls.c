#include "security_acls.h"

bool security_check_permission(struct security_acl *acl, uint32_t accessor_uid, uint32_t requested_access) {
    if (accessor_uid == 0) return true; // Root / Supervisor bypasses checks
    if (acl->owner_uid == accessor_uid) {
        return (acl->permissions & requested_access) != 0;
    }
    return false;
}
