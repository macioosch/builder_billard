object Form1: TForm1
  Left = 292
  Top = 167
  Width = 864
  Height = 623
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
    Width = 701
    Height = 578
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
    Height = 578
    Align = alLeft
    TabOrder = 0
    object CanShootPaintBox: TPaintBox
      Left = 16
      Top = 56
      Width = 17
      Height = 17
      OnPaint = CanShootPaintBoxPaint
    end
    object CanShootLabel: TLabel
      Left = 41
      Top = 56
      Width = 93
      Height = 16
      Caption = 'CanShootLabel'
      Color = clBtnFace
      ParentColor = False
    end
    object Label1: TLabel
      Left = 9
      Top = 81
      Width = 120
      Height = 16
      Caption = 'Total kinetic energy:'
    end
    object Label2: TLabel
      Left = 8
      Top = 184
      Width = 76
      Height = 16
      Caption = 'step / ball R:'
    end
    object Label3: TLabel
      Left = 8
      Top = 240
      Width = 102
      Height = 16
      Caption = 'Sim distance [m]:'
    end
    object LabelDS: TLabel
      Left = 96
      Top = 184
      Width = 53
      Height = 16
      Caption = 'LabelDS'
    end
    object LabelSteps: TLabel
      Left = 112
      Top = 240
      Width = 69
      Height = 16
      Caption = 'LabelSteps'
    end
    object PaintBoxBalls: TPaintBox
      Left = 0
      Top = 336
      Width = 145
      Height = 241
      OnPaint = PaintBoxBallsPaint
    end
    object EnergyBar: TProgressBar
      Left = 9
      Top = 104
      Width = 128
      Height = 33
      Min = 0
      Max = 1000
      Smooth = True
      Step = 1
      TabOrder = 1
    end
    object Button1: TButton
      Left = 8
      Top = 12
      Width = 129
      Height = 30
      Caption = 'New game'
      TabOrder = 0
      OnClick = Button1Click
    end
    object ScrollBarDS: TScrollBar
      Left = 8
      Top = 208
      Width = 129
      Height = 21
      Min = 10
      PageSize = 0
      Position = 50
      TabOrder = 2
      OnChange = SBChange
    end
    object ScrollBarSteps: TScrollBar
      Left = 8
      Top = 264
      Width = 129
      Height = 21
      PageSize = 0
      Position = 20
      TabOrder = 3
      OnChange = SBChange
    end
    object CheckBox1: TCheckBox
      Left = 16
      Top = 304
      Width = 113
      Height = 17
      Caption = 'Alternative aim'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
    object Button2: TButton
      Left = 8
      Top = 152
      Width = 129
      Height = 25
      Caption = 'Replace white ball'
      TabOrder = 5
      OnClick = Button2Click
    end
  end
  object PaintTimer: TTimer
    Interval = 20
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
