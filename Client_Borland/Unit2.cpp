//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
if(Form1->Tag==0) FullScreenCheck->Checked = true;
else FullScreenCheck->Checked = false;
}
//---------------------------------------------------------------------------
//                              ������ �� �����
//---------------------------------------------------------------------------
void __fastcall TForm2::CuncelButtonClick(TObject *Sender)
{
        // ������
ModalResult = mrCancel;         //Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::OkButtonClick(TObject *Sender)
{
        // Ok
        // ������������� ��������� ����� (�������/�������������)
if(FullScreenCheck->Checked==true) {
        Form1->Tag = 0;
        Form1->BorderStyle = bsSizeable;
        }
else {
        Form1->Tag = 1;
        Form1->BorderStyle = bsNone;
        }
        // ������� ��������� ������ ��� �����������
ClOutputRegStruc* ClOutputReg = new ClOutputRegStruc();
StrCopy(ClOutputReg->Id,(NameBox->Text).c_str());
StrCopy(ClOutputReg->Password,(PasswordBox->Text).c_str());
/*
        // �� ��������� ��������� ������� ���� � ������� ��� ���������
AnsiString Id = NameBox->Text;
FILE *F;
if((F=fopen(("OutputFiles\\"+Id+"-01.cvn").c_str(),"wb"))==NULL) {
        Form1->Log->Add("���������� ������� ���� ��� �������� ������ �������");
        Close();
        }
fwrite(ClOutputReg,sizeof(*ClOutputReg),1,F);
fclose(F);
*/
delete ClOutputReg;
/*
        // ������� ���� ����� �������� ������
if((F=fopen(("OutputFiles\\"+Id+"-00.cvn").c_str(),"wb"))==NULL) {
        Form1->Log->Add("���������� ������� ���� ��� �������� ������ �������");
        DeleteFile("OutputFiles\\-01.cvn");  // ���� �������� - ������� ����� ��������� �����
        Close();
        }
fclose(F);
Form1->Log->Add("������� ����� ��� ��������� �� �������");
*/
        // �������� ������ �� ������
//Form1->SendButton->Click();
        // �������� ������ - ��������� �������� ������ �� ������ ��������� ���
//Form1->Timer->Enabled = true;
        // ������� �����
ModalResult = mrOk;             //Close();

}
//---------------------------------------------------------------------------
