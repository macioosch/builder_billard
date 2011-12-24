object Form1: TForm1
  Left = 830
  Top = 477
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
    OnMouseUp = PaintBoxMouseUp
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
      Top = 80
      Width = 17
      Height = 17
      OnPaint = CanShootPaintBoxPaint
    end
    object CanShootLabel: TLabel
      Left = 40
      Top = 80
      Width = 93
      Height = 16
      Caption = 'CanShootLabel'
    end
    object Label1: TLabel
      Left = 8
      Top = 120
      Width = 120
      Height = 16
      Caption = 'Total kinetic energy:'
    end
    object EnergyBar: TProgressBar
      Left = 8
      Top = 144
      Width = 129
      Height = 33
      Min = 0
      Max = 1000
      Smooth = True
      Step = 1
      TabOrder = 1
    end
    object Button1: TButton
      Left = 4
      Top = 20
      Width = 133
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
    OnTimer = ShootingTimerTimer
    Left = 40
    Top = 8
  end
  object ComputeTimer: TTimer
    Enabled = False
    Interval = 2
    OnTimer = ComputeTimerTimer
    Left = 72
    Top = 8
  end
end
