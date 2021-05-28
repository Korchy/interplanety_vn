object Form2: TForm2
  Left = 212
  Top = 139
  Width = 326
  Height = 94
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #1042#1042#1045#1044#1048#1058#1045' ID '#1080' '#1055#1040#1056#1054#1051#1068' '#1044#1051#1071' '#1040#1050#1058#1048#1042#1040#1062#1048#1048' '#1042' '#1057#1048#1057#1058#1045#1052#1045
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Times New Roman'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 40
    Top = 6
    Width = 16
    Height = 14
    Caption = 'ID:'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Times New Roman'
    Font.Style = [fsItalic]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 30
    Width = 47
    Height = 14
    Caption = #1055#1040#1056#1054#1051#1068':'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Times New Roman'
    Font.Style = [fsItalic]
    ParentFont = False
  end
  object NameBox: TEdit
    Left = 64
    Top = 2
    Width = 169
    Height = 22
    MaxLength = 20
    TabOrder = 0
    Text = '98456'
  end
  object PasswordBox: TEdit
    Left = 64
    Top = 26
    Width = 169
    Height = 22
    MaxLength = 10
    PasswordChar = '*'
    TabOrder = 1
    Text = '191919'
  end
  object OkButton: TButton
    Left = 240
    Top = 2
    Width = 65
    Height = 22
    Caption = 'OK'
    TabOrder = 2
    OnClick = OkButtonClick
  end
  object CuncelButton: TButton
    Left = 240
    Top = 26
    Width = 65
    Height = 22
    Caption = #1054#1058#1052#1045#1053#1040
    TabOrder = 3
    OnClick = CuncelButtonClick
  end
  object FullScreenCheck: TCheckBox
    Left = 64
    Top = 50
    Width = 121
    Height = 17
    Caption = #1054#1050#1054#1053#1053#1067#1049' '#1056#1045#1046#1048#1052
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Times New Roman'
    Font.Style = [fsItalic]
    ParentFont = False
    TabOrder = 4
  end
end
