object Form1: TForm1
  Left = 236
  Top = 144
  Width = 873
  Height = 562
  Caption = 'Maciej Chudak'#39's Simple Billard Game'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox: TPaintBox
    Left = 145
    Top = 0
    Width = 712
    Height = 524
    Align = alClient
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 145
    Height = 524
    Align = alLeft
    TabOrder = 0
    object Button1: TButton
      Left = 16
      Top = 16
      Width = 113
      Height = 25
      Caption = 'New game'
      TabOrder = 0
    end
  end
end
