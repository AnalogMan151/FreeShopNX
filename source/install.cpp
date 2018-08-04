#include "common.hpp"

Service g_esSrv;
u64 g_esRefCnt;
Service g_nsAppManSrv, g_nsGetterSrv;
u64 g_nsRefCnt;

Result esInitialize(void)
{
    atomicIncrement64(&g_esRefCnt);
    Result rc = smGetService(&g_esSrv, "es");
    return rc;
}

void esExit(void)
{
    if (atomicDecrement64(&g_esRefCnt) == 0)
    {
        serviceClose(&g_esSrv);
    }
}

static Result _nsGetInterface(Service *srv_out, u64 cmd_id)
{
    IpcCommand c;
    ipcInitialize(&c);

    struct RAW
    {
        u64 magic;
        u64 cmd_id;
    } * raw;

    raw = (struct RAW *)ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = cmd_id;

    Result rc = serviceIpcDispatch(&g_nsGetterSrv);

    if (R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct RESP
        {
            u64 magic;
            u64 result;
        } *resp = (struct RESP *)r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc))
        {
            serviceCreate(srv_out, r.Handles[0]);
        }
    }

    return rc;
}

Result nsextInitialize(void)
{
    Result rc = 0;

    atomicIncrement64(&g_nsRefCnt);

    if (serviceIsActive(&g_nsGetterSrv) || serviceIsActive(&g_nsAppManSrv))
        return 0;

    if (!kernelAbove300())
        return smGetService(&g_nsAppManSrv, "ns:am");

    rc = smGetService(&g_nsGetterSrv, "ns:am2"); //TODO: Support the other services?(Only useful when ns:am2 isn't accessible)
    if (R_FAILED(rc))
        return rc;

    rc = _nsGetInterface(&g_nsAppManSrv, 7996);

    if (R_FAILED(rc))
        serviceClose(&g_nsGetterSrv);

    return rc;
}

void nsextExit(void)
{
    if (atomicDecrement64(&g_nsRefCnt) == 0)
    {
        serviceClose(&g_nsAppManSrv);
        if (!kernelAbove300())
            return;

        serviceClose(&g_nsGetterSrv);
    }
}

Result esImportTicket(void const *tikBuf, size_t tikSize, void const *certBuf, size_t certSize)
{
    IpcCommand c;
    ipcInitialize(&c);
    ipcAddSendBuffer(&c, tikBuf, tikSize, BufferType_Normal);
    ipcAddSendBuffer(&c, certBuf, certSize, BufferType_Normal);

    struct RAW
    {
        u64 magic;
        u64 cmd_id;
    } * raw;

    raw = (struct RAW *)ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 1;

    Result rc = serviceIpcDispatch(&g_esSrv);

    if (R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct RESP
        {
            u64 magic;
            u64 result;
        } *resp = (struct RESP *)r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nsDownloadApplication(u64 tid, u32 unk, u8 storageId)
{
    IpcCommand c;
    ipcInitialize(&c);

    struct RAW
    {
        u64 magic;
        u64 cmd_id;
        u32 storageId;
        u32 unk;
        u64 tid;
    } * raw;

    raw = (struct RAW *)ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 82;
    raw->storageId = storageId;
    raw->unk = unk;
    raw->tid = tid;

    Result rc = serviceIpcDispatch(&g_nsAppManSrv);

    if (R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct RESP
        {
            u64 magic;
            u64 result;
        } *resp = (struct RESP *)r.Raw;

        rc = resp->result;
    }

    return rc;
}

bool installTitle(void)
{
    // TODO: Implement Ticket+Cert install and begin download
    return true;
}