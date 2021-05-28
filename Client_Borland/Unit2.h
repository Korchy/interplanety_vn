//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <stdio.h>      // Для чтения/записи в двоичные файлы
#include "Unit1.h"      // Для связи с главной формой
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TEdit *NameBox;
        TEdit *PasswordBox;
        TButton *OkButton;
        TButton *CuncelButton;
        TCheckBox *FullScreenCheck;
        void __fastcall CuncelButtonClick(TObject *Sender);
        void __fastcall OkButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);

        // Переменные
        struct ClOutputRegStruc {       // Данные отправляемые для регистрации
                char Id[21];
                char Password[11];
                };
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
 