#include "common.hpp"
#include "configuration.hpp"
#include "globals.hpp"

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

unsigned long int byteswap(unsigned long int x)
{
    x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
    x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
    x = (x & 0x00FF00FF00FF00FF) << 8 | (x & 0xFF00FF00FF00FF00) >> 8;
    return x;
}

Result installTikCert(u64 tid, u8 mkey, u64 tkeyh, u64 tkeyl)
{
    tid = byteswap(tid);
    tkeyh = byteswap(tkeyh);
    tkeyl = byteswap(tkeyl);

    static const std::string cert = CERTIFICATE_DATA;

    size_t certBuf_size = cert.length() / 2;
    char *certBuf = new char[certBuf_size];
    for (size_t i = 0, j = 0; j < certBuf_size; i += 2, j++)
    {
        certBuf[j] = std::stoi(cert.substr(i, 2), nullptr, 16);
    }

    static const std::string ticket = TICKET_DATA;

    size_t tikBuf_size = ticket.length() / 2;
    char *tikBuf = new char[tikBuf_size];
    for (size_t i = 0, j = 0; j < tikBuf_size; i += 2, j++)
    {
        tikBuf[j] = std::stoi(ticket.substr(i, 2), nullptr, 16);
    }

    // patch TIK with title data
    memcpy(tikBuf + 0x180, &tkeyh, 8);
    memcpy(tikBuf + 0x188, &tkeyl, 8);
    memcpy(tikBuf + 0x286, &mkey, 1);
    memcpy(tikBuf + 0x2A0, &tid, 8);
    memcpy(tikBuf + 0x2AF, &mkey, 1);

    Result rc = esImportTicket(tikBuf, tikBuf_size, certBuf, certBuf_size);
    return rc;
}

Result installTitle(u64 id)
{
    Result rc = 0;
    rc = nsDownloadApplication(id, 0, g_storageID);
    return rc;
}

bool installTitle(void)
{
    u64 tid = g_titleList[g_idselected].titleID;
    u8 mkey = g_titleList[g_idselected].mKey;
    u64 tkeyh = g_titleList[g_idselected].titleKeyUpper;
    u64 tkeyl = g_titleList[g_idselected].titleKeyLower;

    if (R_FAILED(installTikCert(tid, mkey, tkeyh, tkeyl)))
        return false;
    if (R_FAILED(installTitle(tid)))
        return false;
    return true;
}