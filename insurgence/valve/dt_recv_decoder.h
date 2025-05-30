#pragma once

class CRecvDecoder
{
public:
	void* m_pTable; // TODO: RecvTable*
	void* m_pClientSendTable; // TODO: CClientSendTable*

	// This is from the data that we've received from the server.
	void* m_Precalc; // TODO: CSendTablePrecalc

	// This mirrors m_Precalc.m_Props.
	void* m_Props; // TODO: CUtlVector<const RecvProp*>
	void* m_DatatableProps; // TODO: CUtlVector<const RecvProp*>

	void* m_pDTITable; // TODO: CDTIRecvTable*
};
