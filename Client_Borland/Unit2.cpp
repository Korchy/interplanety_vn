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
//                              КНОПКИ НА ФОРМЕ
//---------------------------------------------------------------------------
void __fastcall TForm2::CuncelButtonClick(TObject *Sender)
{
        // Отмена
ModalResult = mrCancel;         //Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::OkButtonClick(TObject *Sender)
{
        // Ok
        // Устанавливаем выбранный режим (оконный/полноэкранный)
if(FullScreenCheck->Checked==true) {
        Form1->Tag = 0;
        Form1->BorderStyle = bsSizeable;
        }
else {
        Form1->Tag = 1;
        Form1->BorderStyle = bsNone;
        }
        // Создаем структуру данных для регистрации
ClOutputRegStruc* ClOutputReg = new ClOutputRegStruc();
StrCopy(ClOutputReg->Id,(NameBox->Text).c_str());
StrCopy(ClOutputReg->Password,(PasswordBox->Text).c_str());
/*
        // На основании структуры создать файл с данными для активации
AnsiString Id = NameBox->Text;
FILE *F;
if((F=fopen(("OutputFiles\\"+Id+"-01.cvn").c_str(),"wb"))==NULL) {
        Form1->Log->Add("Невозможно создать файл для передачи данных серверу");
        Close();
        }
fwrite(ClOutputReg,sizeof(*ClOutputReg),1,F);
fclose(F);
*/
delete ClOutputReg;
/*
        // Создать файл конца передачи данных
if((F=fopen(("OutputFiles\\"+Id+"-00.cvn").c_str(),"wb"))==NULL) {
        Form1->Log->Add("Невозможно создать файл для передачи данных серверу");
        DeleteFile("OutputFiles\\-01.cvn");  // Если неудачно - удалить ранее созданные файлы
        Close();
        }
fclose(F);
Form1->Log->Add("Созданы файлы для активации на сервере");
*/
        // Отправка данных на сервер
//Form1->SendButton->Click();
        // Включить таймер - обработку проверки сделал ли сервер следующий ход
//Form1->Timer->Enabled = true;
        // Закрыть форму
ModalResult = mrOk;             //Close();

}
//---------------------------------------------------------------------------
