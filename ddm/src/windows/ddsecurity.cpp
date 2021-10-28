
#include "ddsecurity.h"
#include <sddl.h>

BEG_NSP_DDM

DWORD create_low_sa(SECURITY_ATTRIBUTES& secAttr)
{
    do {
        secAttr.nLength = sizeof(SECURITY_DESCRIPTOR);

        // "S:(ML;;NW;;;LW)" this means "low integrity"
        PSECURITY_DESCRIPTOR pSD = NULL;
        if (!::ConvertStringSecurityDescriptorToSecurityDescriptorW(L"S:(ML;;NW;;;LW)", SDDL_REVISION_1, &pSD, NULL)) {
            break;
        }

        BOOL fSaclPresent = FALSE;
        BOOL fSaclDefaulted = FALSE;
        PACL pSacl = NULL;
        if (!::GetSecurityDescriptorSacl(pSD, &fSaclPresent, &pSacl, &fSaclDefaulted)) {
            break;
        }

        if (!::InitializeSecurityDescriptor(secAttr.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)) {
            break;
        }

        if (!::SetSecurityDescriptorSacl(secAttr.lpSecurityDescriptor, TRUE, pSacl, FALSE)) {
            break;
        }

        return ERROR_SUCCESS;
    } while (0);
    return ::GetLastError();
}
END_NSP_DDM