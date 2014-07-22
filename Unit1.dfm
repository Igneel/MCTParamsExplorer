object Form1: TForm1
  Left = 270
  Top = 130
  Width = 1186
  Height = 725
  Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' '#1092#1086#1090#1086#1087#1088#1086#1074#1086#1076#1080#1084#1086#1089#1090#1080' '#1080' '#1092#1086#1090#1086#1084#1072#1075#1085#1080#1090#1085#1086#1075#1086' '#1101#1092#1092#1077#1082#1090#1072
  Color = clBtnFace
  Constraints.MinHeight = 630
  Constraints.MinWidth = 700
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object LabelChan1: TLabel
    Left = 16
    Top = 560
    Width = 240
    Height = 82
    Caption = '0.000000'
    Color = clBackground
    Constraints.MaxWidth = 240
    Constraints.MinWidth = 240
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -67
    Font.Name = 'Digiface'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelChan2: TLabel
    Left = 272
    Top = 560
    Width = 240
    Height = 82
    Caption = '0.000000'
    Color = clBackground
    Constraints.MaxWidth = 240
    Constraints.MinWidth = 240
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -67
    Font.Name = 'Digiface'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelChan3: TLabel
    Left = 528
    Top = 560
    Width = 240
    Height = 82
    Caption = '0.000000'
    Color = clBackground
    Constraints.MaxWidth = 240
    Constraints.MinWidth = 240
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -67
    Font.Name = 'Digiface'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label31: TLabel
    Left = 16
    Top = 536
    Width = 135
    Height = 22
    Alignment = taCenter
    Caption = '1'#1081' '#1082#1072#1085#1072#1083'('#1061#1086#1083#1083'):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label32: TLabel
    Left = 528
    Top = 536
    Width = 136
    Height = 22
    Alignment = taCenter
    Caption = '3'#1081' '#1082#1072#1085#1072#1083'('#1055#1086#1083#1077'):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label33: TLabel
    Left = 272
    Top = 536
    Width = 227
    Height = 22
    Alignment = taCenter
    Caption = '2'#1081' '#1082#1072#1085#1072#1083'('#1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077'):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object PC: TPageControl
    Left = 0
    Top = 0
    Width = 777
    Height = 521
    ActivePage = Resistance
    MultiLine = True
    TabIndex = 1
    TabOrder = 0
    object FirstTab: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      object Label2: TLabel
        Left = 408
        Top = 16
        Width = 72
        Height = 13
        Caption = #1063#1072#1089#1090#1086#1090#1072' ('#1082#1043#1094'):'
      end
      object Label4: TLabel
        Left = 488
        Top = 16
        Width = 150
        Height = 13
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1083#1086#1082#1072' '#1085#1072' '#1086#1076#1080#1085' '#1082#1072#1085#1072#1083':'
      end
      object Label3: TLabel
        Left = 408
        Top = 64
        Width = 110
        Height = 13
        Caption = #1050#1072#1085#1072#1083#1099' '#1080' '#1080#1093' '#1088#1077#1078#1080#1084#1099':'
      end
      object Label1: TLabel
        Left = 536
        Top = 128
        Width = 85
        Height = 13
        Caption = #1052#1072#1075#1085#1090#1080#1085#1086#1077' '#1087#1086#1083#1077'.'
      end
      object Label5: TLabel
        Left = 536
        Top = 104
        Width = 81
        Height = 13
        Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077'.'
      end
      object Label6: TLabel
        Left = 536
        Top = 80
        Width = 77
        Height = 13
        Caption = #1069#1092#1092#1077#1082#1090' '#1061#1086#1083#1083#1072'.'
      end
      object Chart1: TChart
        Left = 0
        Top = 0
        Width = 393
        Height = 329
        AnimatedZoom = True
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object Series1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series3: TLineSeries
          Marks.ArrowLength = 0
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series4: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series5: TBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = True
          SeriesColor = clWhite
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object Memo1: TMemo
        Left = 160
        Top = 416
        Width = 481
        Height = 73
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object uiControl: TButton
        Left = 408
        Top = 192
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 2
        OnClick = uiControlClick
      end
      object uiBlockSize: TEdit
        Left = 488
        Top = 32
        Width = 65
        Height = 21
        TabOrder = 3
        Text = '256'
        OnKeyPress = Edit1KeyPress
      end
      object uiFrenq: TEdit
        Left = 408
        Top = 32
        Width = 65
        Height = 21
        TabOrder = 4
        Text = '400'
        OnKeyPress = Edit1KeyPress
      end
      object bClear: TButton
        Left = 408
        Top = 224
        Width = 75
        Height = 25
        Caption = 'Clear'
        TabOrder = 5
        OnClick = bClearClick
      end
      object bTest: TButton
        Left = 496
        Top = 192
        Width = 75
        Height = 25
        Caption = #1053#1077' '#1085#1072#1078#1080#1084#1072#1090#1100'!'
        TabOrder = 6
      end
      object Button10: TButton
        Left = 496
        Top = 224
        Width = 75
        Height = 25
        Caption = 'Button10'
        TabOrder = 7
        OnClick = Button10Click
      end
      object CheckBox1: TCheckBox
        Left = 16
        Top = 336
        Width = 121
        Height = 17
        Caption = #1052#1077#1076#1080#1072#1085#1085#1099#1081' '#1092#1080#1083#1100#1090#1088
        Checked = True
        State = cbChecked
        TabOrder = 8
      end
      object bApplyADCSettings: TButton
        Left = 408
        Top = 160
        Width = 163
        Height = 25
        Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080' '#1040#1062#1055
        TabOrder = 9
        OnClick = bApplyADCSettingsClick
      end
      object ComboBox1: TComboBox
        Left = 456
        Top = 80
        Width = 73
        Height = 21
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 10
        Text = '2,5'#1042
        Items.Strings = (
          '10'#1042
          '2,5'#1042
          '0,625'#1042
          '0,156'#1042)
      end
      object ComboBox2: TComboBox
        Left = 456
        Top = 104
        Width = 73
        Height = 21
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 11
        Text = '2,5'#1042
        Items.Strings = (
          '10'#1042
          '2,5'#1042
          '0,625'#1042
          '0,156'#1042)
      end
      object ComboBox3: TComboBox
        Left = 456
        Top = 128
        Width = 73
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 12
        Text = '0,625'#1042
        Items.Strings = (
          '10'#1042
          '2,5'#1042
          '0,625'#1042
          '0,156'#1042)
      end
      object ComboBox4: TComboBox
        Left = 408
        Top = 80
        Width = 41
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 13
        Text = '1'
        OnChange = ComboBox5Change
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10'
          '11'
          '12'
          '13'
          '14'
          '15'
          '16')
      end
      object ComboBox5: TComboBox
        Left = 408
        Top = 104
        Width = 41
        Height = 21
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 14
        Text = '2'
        OnChange = ComboBox5Change
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10'
          '11'
          '12'
          '13'
          '14'
          '15'
          '16')
      end
      object ComboBox6: TComboBox
        Left = 408
        Top = 128
        Width = 41
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 15
        Text = '3'
        OnChange = ComboBox5Change
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10'
          '11'
          '12'
          '13'
          '14'
          '15'
          '16')
      end
    end
    object Resistance: TTabSheet
      Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077
      ImageIndex = 1
      object Label7: TLabel
        Left = 320
        Top = 328
        Width = 96
        Height = 13
        Caption = #1048#1085#1090#1077#1088#1074#1072#1083' '#1087#1086' '#1087#1086#1083#1102':'
      end
      object ChartResist: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          #1043#1088#1072#1092#1080#1082' '#1084#1072#1075#1085#1080#1090#1086#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1103)
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesRes1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesRes2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTRes: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object uiResControl: TButton
        Left = 384
        Top = 200
        Width = 107
        Height = 25
        Caption = #1053#1072#1095#1072#1090#1100' '#1079#1072#1087#1080#1089#1100
        TabOrder = 1
        OnClick = uiControlClick
      end
      object Memo2: TMemo
        Left = 440
        Top = 328
        Width = 129
        Height = 89
        TabOrder = 2
      end
      object ResCurveIndex: TRadioGroup
        Left = 384
        Top = 8
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1086#1083#1086#1078#1080#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1054#1090#1088#1080#1094#1072#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1057#1086#1074#1084#1077#1097#1077#1085#1085#1086#1077' '#1042)
        TabOrder = 3
        OnClick = ResCurveIndexClick
      end
      object bFilterRes: TButton
        Left = 512
        Top = 232
        Width = 145
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088' '#1080' '#1069#1082#1089#1090#1088#1072#1087#1086#1083#1103#1094#1080#1103
        TabOrder = 4
        OnClick = bFilterResClick
      end
      object bClearPlotsRes: TButton
        Left = 384
        Top = 232
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 5
        OnClick = bClearPlotsResClick
      end
      object Interval1: TEdit
        Left = 8
        Top = 432
        Width = 49
        Height = 21
        TabOrder = 6
        Text = '0,02'
        OnKeyPress = Edit1KeyPress
      end
      object Interval2: TEdit
        Left = 64
        Top = 432
        Width = 49
        Height = 21
        TabOrder = 7
        Text = '0,15'
        OnKeyPress = Edit1KeyPress
      end
      object ImpulsKiller: TButton
        Left = 8
        Top = 456
        Width = 105
        Height = 25
        Caption = #1059#1073#1088#1072#1090#1100' '#1074#1089#1087#1083#1077#1089#1082
        Enabled = False
        TabOrder = 8
        OnClick = ImpulsKillerClick
      end
      object Button13: TButton
        Left = 120
        Top = 456
        Width = 75
        Height = 25
        Caption = #1054#1094#1077#1085#1080#1090#1100' '#1096#1091#1084
        TabOrder = 9
        OnClick = Button13Click
      end
      object Button1: TButton
        Left = 512
        Top = 200
        Width = 145
        Height = 25
        Caption = #1054#1073#1098#1077#1076#1080#1085#1080#1090#1100' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080
        TabOrder = 10
        OnClick = Button1Click
      end
      object Memo6: TMemo
        Left = 8
        Top = 336
        Width = 161
        Height = 65
        Lines.Strings = (
          #1050#1088#1072#1089#1085#1099#1081' - '#1080#1079#1084#1077#1088#1077#1085#1085#1099#1081
          #1057#1080#1085#1080#1081' - '#1092#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1081' '
          #1063#1077#1088#1085#1099#1081' - '
          #1101#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1081)
        TabOrder = 11
      end
      object uiShiftValue: TLabeledEdit
        Left = 224
        Top = 344
        Width = 89
        Height = 21
        EditLabel.Width = 86
        EditLabel.Height = 13
        EditLabel.Caption = #1042#1077#1083#1080#1095#1080#1085#1072' '#1089#1076#1074#1080#1075#1072
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 12
        Text = '0,01'
        OnKeyPress = Edit1KeyPress
      end
      object bResShiftCurve: TButton
        Left = 224
        Top = 448
        Width = 121
        Height = 25
        Caption = #1057#1076#1074#1080#1085#1091#1090#1100' '#1075#1088#1072#1092#1080#1082
        TabOrder = 13
        OnClick = bResShiftCurveClick
      end
      object uiDataKind: TRadioGroup
        Left = 224
        Top = 368
        Width = 185
        Height = 81
        Caption = #1042#1080#1076' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080':'
        ItemIndex = 0
        Items.Strings = (
          #1058#1077#1082#1091#1097#1080#1077' '#1076#1072#1085#1085#1099#1077
          #1060#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
          #1069#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
          #1059#1089#1088#1077#1076#1077#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077)
        TabOrder = 14
      end
      object uiLeftBound: TComboBox
        Left = 320
        Top = 344
        Width = 49
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 15
        Text = '-2'
        Items.Strings = (
          '-2'
          '-1'
          '0'
          '1'
          '2')
      end
      object uiRightBound: TComboBox
        Left = 368
        Top = 344
        Width = 49
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 16
        Text = '0'
        Items.Strings = (
          '-2'
          '-1'
          '0'
          '1'
          '2')
      end
      object GroupBox2: TGroupBox
        Left = 552
        Top = 8
        Width = 193
        Height = 145
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1080
        TabOrder = 17
        object Label21: TLabel
          Left = 6
          Top = 100
          Width = 99
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
        end
        object Label15: TLabel
          Left = 23
          Top = 24
          Width = 82
          Height = 13
          Alignment = taCenter
          Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
        end
        object Label20: TLabel
          Left = 16
          Top = 76
          Width = 89
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
        end
        object Label19: TLabel
          Left = 16
          Top = 52
          Width = 89
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
        end
        object eLengthFilterRes: TEdit
          Left = 112
          Top = 16
          Width = 73
          Height = 21
          TabOrder = 0
          Text = '2'
          OnChange = eLengthFilterResChange
          OnKeyPress = Edit1KeyPress
        end
        object eBandWidthFRes: TEdit
          Left = 112
          Top = 68
          Width = 73
          Height = 21
          TabOrder = 1
          Text = '15'
          OnKeyPress = Edit1KeyPress
        end
        object eAttenuationFRes: TEdit
          Left = 112
          Top = 92
          Width = 73
          Height = 21
          TabOrder = 2
          Text = '25'
          OnKeyPress = Edit1KeyPress
        end
        object PowPolinomRes: TLabeledEdit
          Left = 112
          Top = 116
          Width = 73
          Height = 21
          EditLabel.Width = 98
          EditLabel.Height = 13
          EditLabel.Caption = #1057#1090#1077#1087#1077#1085#1100' '#1087#1086#1083#1080#1085#1086#1084#1072':'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 3
          Text = '4'
          OnKeyPress = Edit1KeyPress
        end
        object eSamplingFRes: TEdit
          Left = 112
          Top = 44
          Width = 73
          Height = 21
          TabOrder = 4
          Text = '10000'
          OnKeyPress = Edit1KeyPress
        end
      end
      object Button2: TButton
        Left = 512
        Top = 272
        Width = 145
        Height = 25
        Caption = #1041#1083#1086#1095#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103
        TabOrder = 18
        OnClick = Button2Click
      end
    end
    object HallEffect: TTabSheet
      Caption = #1069#1092#1092#1077#1082#1090' '#1061#1086#1083#1083#1072
      ImageIndex = 2
      object Label8: TLabel
        Left = 216
        Top = 336
        Width = 96
        Height = 13
        Caption = #1048#1085#1090#1077#1088#1074#1072#1083' '#1087#1086' '#1087#1086#1083#1102':'
      end
      object ChartHall: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          #1043#1088#1072#1092#1080#1082' '#1101#1092#1092#1077#1082#1090#1072' '#1061#1086#1083#1083#1072)
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesHall1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesHall2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTHall: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object HallCurveIndex: TRadioGroup
        Left = 384
        Top = 8
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1086#1083#1086#1078#1080#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1054#1090#1088#1080#1094#1072#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1057#1086#1074#1084#1077#1097#1077#1085#1085#1086#1077' '#1042)
        TabOrder = 1
        OnClick = HallCurveIndexClick
      end
      object uiHallFeat: TButton
        Left = 416
        Top = 232
        Width = 75
        Height = 25
        Caption = #1055#1086#1076#1075#1086#1085#1082#1072
        TabOrder = 2
        OnClick = uiHallFeatClick
      end
      object uiHallControl: TButton
        Left = 416
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 3
        OnClick = uiControlClick
      end
      object Memo3: TMemo
        Left = 456
        Top = 320
        Width = 185
        Height = 89
        TabOrder = 4
      end
      object uiFFTHall: TButton
        Left = 496
        Top = 232
        Width = 75
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088
        TabOrder = 5
        OnClick = bFilterResClick
      end
      object bClearPlotHall: TButton
        Left = 336
        Top = 328
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 6
        OnClick = bClearPlotHallClick
      end
      object uiHallShiftValue: TLabeledEdit
        Left = 120
        Top = 352
        Width = 89
        Height = 21
        EditLabel.Width = 86
        EditLabel.Height = 13
        EditLabel.Caption = #1042#1077#1083#1080#1095#1080#1085#1072' '#1089#1076#1074#1080#1075#1072
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 7
        Text = '0,01'
        OnKeyPress = Edit1KeyPress
      end
      object uiHallDataKind: TRadioGroup
        Left = 120
        Top = 376
        Width = 185
        Height = 81
        Caption = #1042#1080#1076' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080':'
        ItemIndex = 0
        Items.Strings = (
          #1058#1077#1082#1091#1097#1080#1077' '#1076#1072#1085#1085#1099#1077
          #1060#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
          #1069#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
          #1059#1089#1088#1077#1076#1077#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077)
        TabOrder = 8
      end
      object bShiftHallCurve: TButton
        Left = 120
        Top = 456
        Width = 121
        Height = 25
        Caption = #1057#1076#1074#1080#1085#1091#1090#1100' '#1075#1088#1072#1092#1080#1082
        TabOrder = 9
        OnClick = bShiftHallCurveClick
      end
      object uiHallLeftBound: TComboBox
        Left = 216
        Top = 352
        Width = 49
        Height = 21
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 10
        Text = '-2'
        Items.Strings = (
          '-2'
          '-1'
          '0'
          '1'
          '2')
      end
      object uiHallRightBound: TComboBox
        Left = 264
        Top = 352
        Width = 49
        Height = 21
        ItemHeight = 13
        ItemIndex = 2
        TabOrder = 11
        Text = '0'
        Items.Strings = (
          '-2'
          '-1'
          '0'
          '1'
          '2')
      end
      object GroupBox3: TGroupBox
        Left = 552
        Top = 16
        Width = 201
        Height = 145
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1080
        TabOrder = 12
        object Label16: TLabel
          Left = 23
          Top = 24
          Width = 82
          Height = 13
          Alignment = taCenter
          Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
        end
        object Label22: TLabel
          Left = 16
          Top = 48
          Width = 89
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
        end
        object Label23: TLabel
          Left = 16
          Top = 72
          Width = 89
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
        end
        object Label24: TLabel
          Left = 6
          Top = 96
          Width = 99
          Height = 13
          Alignment = taCenter
          Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
        end
        object PowPolinomHall: TLabeledEdit
          Left = 112
          Top = 112
          Width = 73
          Height = 21
          EditLabel.Width = 98
          EditLabel.Height = 13
          EditLabel.Caption = #1057#1090#1077#1087#1077#1085#1100' '#1087#1086#1083#1080#1085#1086#1084#1072':'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 0
          Text = '4'
          OnKeyPress = Edit1KeyPress
        end
        object eAttenuationFHall: TEdit
          Left = 112
          Top = 88
          Width = 73
          Height = 21
          TabOrder = 1
          Text = '25'
          OnKeyPress = Edit1KeyPress
        end
        object eBandWidthFHall: TEdit
          Left = 112
          Top = 64
          Width = 73
          Height = 21
          TabOrder = 2
          Text = '15'
          OnKeyPress = Edit1KeyPress
        end
        object eSamplingFHall: TEdit
          Left = 112
          Top = 40
          Width = 73
          Height = 21
          TabOrder = 3
          Text = '10000'
          OnKeyPress = Edit1KeyPress
        end
        object eLengthFilterHall: TEdit
          Left = 112
          Top = 16
          Width = 73
          Height = 21
          TabOrder = 4
          Text = '2'
          OnChange = eLengthFilterHallChange
          OnKeyPress = Edit1KeyPress
        end
      end
    end
    object Faradey: TTabSheet
      Caption = #1060#1086#1087#1088#1086#1074#1086#1076#1080#1084#1086#1089#1090#1100' ('#1060#1072#1088#1072#1076#1077#1081') '
      ImageIndex = 3
      object Label12: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = #1055#1088#1086#1090#1077#1082#1072#1102#1097#1080#1081' '#1090#1086#1082':'
      end
      object Label13: TLabel
        Left = 384
        Top = 48
        Width = 144
        Height = 13
        Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090' '#1091#1089#1080#1083#1077#1085#1080#1103'(mV):'
      end
      object Label17: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
      end
      object Label25: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
      end
      object Label26: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
      end
      object Label27: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
      end
      object ChartFaradey: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesFaradey1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFaradey2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTFaradey: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out3: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object CurrentFaradey: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 1
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object GainKoefFaradey: TComboBox
        Left = 384
        Top = 64
        Width = 97
        Height = 21
        ItemHeight = 13
        TabOrder = 2
        Items.Strings = (
          '30'
          '10'
          '3'
          '1'
          '0.3'
          '0.1')
      end
      object FaradeyCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1086#1083#1086#1078#1080#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1054#1090#1088#1080#1094#1072#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1057#1086#1074#1084#1077#1097#1077#1085#1085#1086#1077' '#1042)
        TabOrder = 3
      end
      object uiFaradeyControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 4
        OnClick = uiControlClick
      end
      object uiFaradeyFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = #1055#1086#1076#1075#1086#1085#1082#1072
        TabOrder = 5
        OnClick = uiFaradeyFeatClick
      end
      object Memo4: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 6
      end
      object uiFFTFaradey: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088
        TabOrder = 7
      end
      object Button7: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 8
        OnClick = Button7Click
      end
      object Lfilter3: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fd3: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp3: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz3: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 12
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
    end
    object Foygt: TTabSheet
      Caption = #1060#1086#1090#1086#1087#1088#1086#1074#1086#1076#1080#1084#1086#1089#1090#1100' ('#1060#1086#1081#1075#1090')'
      ImageIndex = 4
      object Label10: TLabel
        Left = 384
        Top = 48
        Width = 144
        Height = 13
        Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090' '#1091#1089#1080#1083#1077#1085#1080#1103'(mV):'
      end
      object Label14: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = #1055#1088#1086#1090#1077#1082#1072#1102#1097#1080#1081' '#1090#1086#1082':'
      end
      object Label18: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
      end
      object Label28: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
      end
      object Label29: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
      end
      object Label30: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
      end
      object ChartFoygt: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesFoygt1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFoygt2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTFoygt: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Title = 'SeriesFFTFoygt'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out4: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object GainKoefFoygt: TComboBox
        Left = 384
        Top = 64
        Width = 97
        Height = 21
        ItemHeight = 13
        TabOrder = 1
        Items.Strings = (
          '30'
          '10'
          '3'
          '1'
          '0.3'
          '0.1')
      end
      object CurrentFoygt: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 2
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object FoygtCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1086#1083#1086#1078#1080#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1054#1090#1088#1080#1094#1072#1090#1077#1083#1100#1085#1086#1077' '#1042
          #1057#1086#1074#1084#1077#1097#1077#1085#1085#1086#1077' '#1042)
        TabOrder = 3
      end
      object uiFoygtControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 4
        OnClick = uiControlClick
      end
      object uiFoygtFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = #1055#1086#1076#1075#1086#1085#1082#1072
        TabOrder = 5
        OnClick = uiFoygtFeatClick
      end
      object Memo5: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 6
      end
      object uiFFTFoygt: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088
        TabOrder = 7
      end
      object Button8: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 8
        OnClick = Button8Click
      end
      object Fd4: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Lfilter4: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fp4: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz4: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 12
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 648
    Width = 1170
    Height = 19
    Panels = <
      item
        Text = #1058#1077#1082#1091#1097#1077#1077' '#1089#1086#1089#1090#1086#1103#1085#1080#1077':'
        Width = 110
      end
      item
        Text = #1043#1086#1090#1086#1074#1072' '#1082' '#1088#1072#1073#1086#1090#1077'.'
        Width = 182
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object uiCalculateTenzor: TButton
    Left = 800
    Top = 192
    Width = 97
    Height = 25
    Caption = #1056#1072#1089#1095#1077#1090' '#1090#1077#1085#1079#1086#1088#1086#1074
    TabOrder = 2
    OnClick = Button3Click
  end
  object GroupBox1: TGroupBox
    Left = 800
    Top = 24
    Width = 225
    Height = 153
    Caption = #1054#1087#1080#1089#1072#1085#1080#1077' '#1086#1073#1088#1072#1079#1094#1072
    TabOrder = 3
    object Label35: TLabel
      Left = 21
      Top = 24
      Width = 84
      Height = 13
      Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072' ('#1084#1082#1040'):'
    end
    object uiCurrent: TEdit
      Left = 16
      Top = 40
      Width = 89
      Height = 21
      TabOrder = 0
      Text = '250'
      OnChange = CurrentResChange
      OnKeyPress = Edit1KeyPress
    end
    object uiTemperature: TLabeledEdit
      Left = 16
      Top = 80
      Width = 89
      Height = 21
      EditLabel.Width = 86
      EditLabel.Height = 13
      EditLabel.Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' ('#1050'):'
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 1
      Text = '77'
      OnChange = CurrentResChange
      OnKeyPress = Edit1KeyPress
    end
    object uiInventoryNumber: TLabeledEdit
      Left = 16
      Top = 120
      Width = 89
      Height = 21
      EditLabel.Width = 61
      EditLabel.Height = 13
      EditLabel.Caption = #8470' '#1054#1073#1088#1072#1079#1094#1072':'
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 2
      Text = '123456'
      OnChange = CurrentResChange
    end
    object uiSampleLength: TLabeledEdit
      Left = 120
      Top = 36
      Width = 65
      Height = 21
      EditLabel.Width = 61
      EditLabel.Height = 13
      EditLabel.Caption = #1044#1083#1080#1085#1072' ('#1084#1084'):'
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 3
      Text = '1,24'
      OnChange = CurrentResChange
      OnKeyPress = Edit1KeyPress
    end
    object uiSampleWidth: TLabeledEdit
      Left = 120
      Top = 76
      Width = 73
      Height = 21
      EditLabel.Width = 67
      EditLabel.Height = 13
      EditLabel.Caption = #1064#1080#1088#1080#1085#1072' ('#1084#1084'):'
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 4
      Text = '1,0'
      OnChange = CurrentResChange
      OnKeyPress = Edit1KeyPress
    end
    object uiSampleThickness: TLabeledEdit
      Left = 120
      Top = 116
      Width = 89
      Height = 21
      EditLabel.Width = 80
      EditLabel.Height = 13
      EditLabel.Caption = #1058#1086#1083#1097#1080#1085#1072' ('#1084#1082#1084'):'
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 5
      Text = '12'
      OnChange = CurrentResChange
      OnKeyPress = Edit1KeyPress
    end
  end
  object MainMenu1: TMainMenu
    Top = 440
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N4: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        ShortCut = 16463
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        ShortCut = 16467
        OnClick = N5Click
      end
      object N11: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074#1089#1105
        ShortCut = 49235
        OnClick = N11Click
      end
      object N3: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N3Click
      end
    end
    object N8: TMenuItem
      Caption = #1048#1085#1089#1090#1088#1091#1084#1077#1085#1090#1099
      object N101: TMenuItem
        Caption = #1059#1084#1085#1086#1078#1080#1090#1100' '#1090#1077#1082#1091#1097#1080#1081' '#1075#1088#1072#1092#1080#1082' '#1085#1072' 10'
        Enabled = False
      end
      object N9: TMenuItem
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        ShortCut = 8238
        OnClick = N9Click
      end
      object N10: TMenuItem
        Caption = #1059#1073#1088#1072#1090#1100' '#1074#1089#1087#1083#1077#1089#1082
        Enabled = False
      end
    end
    object N2: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      Enabled = False
    end
    object N6: TMenuItem
      Caption = #1055#1086#1084#1086#1097#1100
      Enabled = False
      object N7: TMenuItem
        Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
        ShortCut = 112
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 24
    Top = 440
  end
  object OpenDialog1: TOpenDialog
    Left = 48
    Top = 440
  end
end
