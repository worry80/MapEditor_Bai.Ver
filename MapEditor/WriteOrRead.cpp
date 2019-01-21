#include"stdafx.h"
#include"WriteOrRead.h"


void WritePntToFile(CFile * PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}

/*����ʱ���ļ���ȡ������*/
void ReadTempFileToPnt(CFile * PntTmpF, int i, PNT_STRU & point)
{
	PntTmpF->Seek(i * sizeof(PNT_STRU), CFile::begin);	//���¶�λָ��
	PntTmpF->Read(&point, sizeof(PNT_STRU));			//��ȡ����
}

/*�������ݴ������ļ�������ʱ�ļ�*/
void ReadPntPermanentFileToTemp(CFile * pntF, CFile * pntTmpF, int & nPnt, int & nLPnt)
{
	PNT_STRU point;
	pntF->Seek(sizeof(VERSION), CFile::begin);	//���ļ�ָ��ŵ��ļ�ͷ����
	pntF->Read(&nPnt, sizeof(int));				//�������������
	pntF->Read(&nLPnt, sizeof(int));			//������߼�����
	for (int i = 0; i < nPnt; i++)
	{
		pntF->Read(&point, sizeof(PNT_STRU));	//�������������
		pntTmpF->Write(&point, sizeof(PNT_STRU));//���д�������
	}
}

/*����i���ߵ�����д����ʱ�����ļ�*/
void WriteLinNdxToFile(CFile * linTmpNdxF, int i, LIN_NDX_STRU line)
{
	linTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	linTmpNdxF->Write(&line, sizeof(LIN_NDX_STRU));
}

/*���ߵĵ�����д���ļ�*/
void WriteLinDatToFile(CFile * linTmpDatF, long datOff, int i, D_DOT point)
{
	linTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	linTmpDatF->Write(&point, sizeof(D_DOT));
}

/*����ʱ�������ļ��ж�ȡ�ߵĵ�����*/
void ReadTempFileToLinDat(CFile * LinTmpDatF, long datOff, int i, D_DOT & pnt)
{
	LinTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF->Read(&pnt, sizeof(D_DOT));
}

/*����ʱ�������ļ��ж�ȡ������*/
void ReadTempFileToLinNdx(CFile * LinTmpNdxF, int i, LIN_NDX_STRU & LinNdx)
{
	LinTmpNdxF->Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
}

/*���ߵ������͵�����д�������ļ�*/
void WriteTempToLinPermanentFile(CFile * LinF, CFile * LinTmpDatF, CFile * LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU)*nLin;
	LinF->Write(&LinVer, sizeof(VERSION));		//д��汾��Ϣ
	LinF->Write(&nLin, sizeof(int));			//д��������
	LinF->Write(&nLLin, sizeof(int));			//д���߼���
	for (int i = 0; i < nLin; i++)
	{
		//����ʱ�������ļ��ж�ȡ������
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF->Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			//����ʱ�������ļ��ж�ȡ�ߵĵ�����
			ReadTempFileToLinDat(LinTmpDatF, TempLinNdx.datOff, j, Pnt);
			//���ߵĵ�����д�������ļ�
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF->Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		//���ߵ�����д�������ļ�
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinNdxOffset += sizeof(LIN_NDX_STRU);
		LinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
	}
}

/*�������ļ���ȡ�����ݵ���ʱ�ļ�*/
void ReadLinPermanentFileToTemp(CFile*LinF, CFile*LinTmpDatF,
	CFile*LinTmpNdxF, VERSION&LinVer, int&nLin, int&nLLin, long&TmpFLinDatOffset)
{
	LinF->Seek(0, CFile::begin);
	LinF->Read(&LinVer, sizeof(VERSION));
	LinF->Read(&nLin, sizeof(int));		//����������
	LinF->Read(&nLLin, sizeof(int));	//���߼�����
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU)*nLin;
	TmpFLinDatOffset = 0;
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	for (int i = 0; i < nLin; i++)
	{
		LinF->Seek(LinNdxOffset, CFile::begin);
		LinF->Read(&TempLinNdx, sizeof(LIN_NDX_STRU));
		LinF->Seek(TempLinNdx.datOff, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			LinF->Read(&Pnt, sizeof(D_DOT));
			LinTmpDatF->Write(&Pnt, sizeof(D_DOT));
		}
		TempLinNdx.datOff = TmpFLinDatOffset;
		LinTmpNdxF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));
		TmpFLinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
		LinDatOffset += (sizeof(D_DOT)*TempLinNdx.dotNum);
		LinNdxOffset += sizeof(LIN_NDX_STRU);
	}
}

/*���ߵ���ʱ�����ļ��и���������*/
void UpdateLin(CFile * LinTmpNdxF, int nLin, LIN_NDX_STRU line)
{
	WriteLinNdxToFile(LinTmpNdxF, nLin, line);
}

/*�����ߵĵ����ݵ���ʱ�ļ�*/
void UpdateLin(CFile * LinTmpNdxF, CFile * LinTmpDatF, int LinNdx, double offset_x, double offset_y)
{
	LIN_NDX_STRU tLin;
	D_DOT pt;
	ReadTempFileToLinNdx(LinTmpNdxF, LinNdx, tLin);
	for (int i = 0; i < tLin.dotNum; ++i)
	{
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Read(&pt, sizeof(D_DOT));
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		LinTmpDatF->Seek(tLin.datOff + i * sizeof(D_DOT), CFile::begin);
		LinTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}

/*�޸ĵ�һ��������*/
void AlterStartLin(CFile * LinTmpNdxF, long subdatOff, int nLine, int subNum)
{
	LIN_NDX_STRU LinNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	LinNdx.datOff = subdatOff;		// �����ߵĵ�����
	LinNdx.dotNum = subNum;			// �����ߵĵ���Ŀ
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}

/*�޸ĵڶ���������*/
void AlterEndLin(CFile * LinTmpNdxF, int nLine)
{
	LIN_NDX_STRU LinNdx;
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	LinNdx.dotNum = 0;				// �ߵĵ���Ϊ0
	LinNdx.isDel = 0;				// ����ɾ�����
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}


