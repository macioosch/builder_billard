object Form1: TForm1
  Left = 293
  Top = 462
  Width = 806
  Height = 576
  Caption = 'Maciej Chudak'#39's Simple Billard Game'
  Color = clBtnFace
  Constraints.MinHeight = 300
  Constraints.MinWidth = 400
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 120
  TextHeight = 16
  object PaintBox: TPaintBox
    Left = 145
    Top = 0
    Width = 643
    Height = 531
    Align = alClient
    OnMouseDown = PaintBoxMouseDown
    OnMouseMove = PaintBoxMouseMove
    OnPaint = PaintBoxPaint
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 145
    Height = 531
    Align = alLeft
    TabOrder = 0
    object CanShootPaintBox: TPaintBox
      Left = 16
      Top = 72
      Width = 17
      Height = 17
      OnPaint = CanShootPaintBoxPaint
    end
    object CanShootLabel: TLabel
      Left = 40
      Top = 72
      Width = 93
      Height = 16
      Caption = 'CanShootLabel'
    end
    object Button1: TButton
      Left = 12
      Top = 20
      Width = 117
      Height = 30
      Caption = 'New game'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object PaintTimer: TTimer
    Interval = 10
    OnTimer = PaintTimerTimer
    Left = 8
    Top = 8
  end
  object ShootingTimer: TTimer
    Enabled = False
    Interval = 1
    Left = 40
    Top = 8
  end
end
