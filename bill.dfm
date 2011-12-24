object Form1: TForm1
  Left = 573
  Top = 217
  Width = 806
  Height = 576
  Caption = 'Maciej Chudak'#39's Simple Billard Game'
  Color = clBtnFace
  Constraints.MinHeight = 300
  Constraints.MinWidth = 400
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
    Left = 118
    Top = 0
    Width = 672
    Height = 538
    Align = alClient
    OnMouseDown = PaintBoxMouseDown
    OnMouseMove = PaintBoxMouseMove
    OnMouseUp = PaintBoxMouseUp
    OnPaint = PaintBoxPaint
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 118
    Height = 538
    Align = alLeft
    TabOrder = 0
    object CanShootPaintBox: TPaintBox
      Left = 13
      Top = 65
      Width = 14
      Height = 14
      OnPaint = CanShootPaintBoxPaint
    end
    object CanShootLabel: TLabel
      Left = 33
      Top = 65
      Width = 73
      Height = 13
      Caption = 'CanShootLabel'
    end
    object Label1: TLabel
      Left = 7
      Top = 98
      Width = 96
      Height = 13
      Caption = 'Total kinetic energy:'
    end
    object EnergyBar: TProgressBar
      Left = 7
      Top = 117
      Width = 104
      Height = 27
      Min = 0
      Max = 1000
      Smooth = True
      Step = 1
      TabOrder = 1
    end
    object Button1: TButton
      Left = 3
      Top = 16
      Width = 108
      Height = 25
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
