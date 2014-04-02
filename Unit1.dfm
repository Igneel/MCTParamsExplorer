object Form1: TForm1
  Left = 207
  Top = 122
  Width = 700
  Height = 530
  Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' '#1092#1086#1090#1086#1087#1088#1086#1074#1086#1076#1080#1084#1086#1089#1090#1080' '#1080' '#1092#1086#1090#1086#1084#1072#1075#1085#1080#1090#1085#1086#1075#1086' '#1101#1092#1092#1077#1082#1090#1072
  Color = clBtnFace
  Constraints.MaxHeight = 530
  Constraints.MaxWidth = 700
  Constraints.MinHeight = 530
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
  object PC: TPageControl
    Left = 0
    Top = 0
    Width = 657
    Height = 441
    ActivePage = HallEffect
    MultiLine = True
    TabIndex = 2
    TabOrder = 0
    object FirstTab: TTabSheet
      Caption = 'FirstTab'
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
        Width = 75
        Height = 13
        Caption = #1056#1072#1079#1084#1077#1088' '#1073#1083#1086#1082#1072':'
      end
      object Label6: TLabel
        Left = 400
        Top = 176
        Width = 43
        Height = 26
        Alignment = taCenter
        Caption = #1050#1072#1085#1072#1083#1099': '#1055#1077#1088#1074#1099#1081':'
        WordWrap = True
      end
      object Label7: TLabel
        Left = 400
        Top = 232
        Width = 39
        Height = 13
        Caption = #1042#1090#1086#1088#1086#1081':'
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
      end
      object Memo1: TMemo
        Left = 424
        Top = 272
        Width = 217
        Height = 129
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object uiControl: TButton
        Left = 472
        Top = 208
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 2
        OnClick = uiControlClick
      end
      object Chan1: TComboBox
        Left = 400
        Top = 208
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 3
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6')
      end
      object Chan2: TComboBox
        Left = 400
        Top = 248
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object uiBlockSize2: TEdit
        Left = 488
        Top = 32
        Width = 65
        Height = 21
        TabOrder = 5
        Text = '1024'
        OnKeyPress = Edit1KeyPress
      end
      object uiFrenq: TEdit
        Left = 408
        Top = 32
        Width = 65
        Height = 21
        TabOrder = 6
        Text = '400'
        OnKeyPress = Edit1KeyPress
      end
      object bClear: TButton
        Left = 472
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Clear'
        TabOrder = 7
        OnClick = bClearClick
      end
      object Memo6: TMemo
        Left = 0
        Top = 336
        Width = 289
        Height = 73
        ScrollBars = ssVertical
        TabOrder = 8
      end
      object bTest: TButton
        Left = 560
        Top = 208
        Width = 75
        Height = 25
        Caption = #1053#1077' '#1085#1072#1078#1080#1084#1072#1090#1100'!'
        TabOrder = 9
        OnClick = bTestClick
      end
      object Button10: TButton
        Left = 312
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Button10'
        TabOrder = 10
        OnClick = Button10Click
      end
    end
    object Resistance: TTabSheet
      Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077
      ImageIndex = 1
      object Label9: TLabel
        Left = 384
        Top = 8
        Width = 116
        Height = 13
        Caption = #1055#1088#1086#1090#1077#1082#1072#1102#1097#1080#1081' '#1090#1086#1082'('#1084#1040'):'
      end
      object Label15: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
      end
      object Label19: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
      end
      object Label20: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
      end
      object Label21: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
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
        Left = 416
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 1
        OnClick = uiControlClick
      end
      object CurrentRes: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 2
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object uiResFeat: TButton
        Left = 416
        Top = 232
        Width = 75
        Height = 25
        Caption = #1055#1086#1076#1075#1086#1085#1082#1072
        Enabled = False
        TabOrder = 3
        OnClick = uiResFeatClick
      end
      object Memo2: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 4
      end
      object ResCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1077#1088#1074#1072#1103' ('#1087#1088#1103#1084#1086#1081' '#1090#1086#1082')'
          #1042#1090#1086#1088#1072#1103' ('#1086#1073#1088#1072#1090#1085#1099#1081' '#1090#1086#1082')')
        TabOrder = 5
      end
      object bFilterRes: TButton
        Left = 496
        Top = 232
        Width = 145
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088' '#1080' '#1069#1082#1089#1090#1088#1072#1087#1086#1083#1103#1094#1080#1103
        TabOrder = 6
        OnClick = bFilterResClick
      end
      object Lfilter1: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 7
        Text = '50'
        OnKeyPress = Edit1KeyPress
      end
      object Fd1: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 8
        Text = '10000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp1: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz1: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
      object bClearPlotsRes: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 11
        OnClick = bClearPlotsResClick
      end
      object Interval1: TEdit
        Left = 48
        Top = 344
        Width = 49
        Height = 21
        TabOrder = 12
        Text = '0,02'
        OnKeyPress = Edit1KeyPress
      end
      object Interval2: TEdit
        Left = 104
        Top = 344
        Width = 49
        Height = 21
        TabOrder = 13
        Text = '0,15'
        OnKeyPress = Edit1KeyPress
      end
      object ImpulsKiller: TButton
        Left = 48
        Top = 368
        Width = 105
        Height = 25
        Caption = #1059#1073#1088#1072#1090#1100' '#1074#1089#1087#1083#1077#1089#1082
        Enabled = False
        TabOrder = 14
        OnClick = ImpulsKillerClick
      end
      object bMultuplyB: TButton
        Left = 168
        Top = 368
        Width = 75
        Height = 25
        Caption = #1093'10'
        Enabled = False
        TabOrder = 15
        OnClick = bMultuplyBClick
      end
      object Button11: TButton
        Left = 344
        Top = 368
        Width = 129
        Height = 25
        Caption = #1054#1087#1090#1080#1084#1072#1083#1100#1085#1099#1081' '#1092#1080#1083#1100#1090#1088
        Enabled = False
        TabOrder = 16
      end
      object Button13: TButton
        Left = 256
        Top = 368
        Width = 75
        Height = 25
        Caption = #1054#1094#1077#1085#1080#1090#1100' '#1096#1091#1084
        TabOrder = 17
        OnClick = Button13Click
      end
    end
    object HallEffect: TTabSheet
      Caption = #1069#1092#1092#1077#1082#1090' '#1061#1086#1083#1083#1072
      ImageIndex = 2
      object Label11: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = #1055#1088#1086#1090#1077#1082#1072#1102#1097#1080#1081' '#1090#1086#1082':'
      end
      object Label16: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
      end
      object Label22: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1076#1080#1089#1082#1088#1077#1090':'
      end
      object Label23: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1087#1091#1089#1082':'
      end
      object Label24: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1079#1072#1090#1091#1093#1072#1085#1080#1103':'
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
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = #1053#1086#1084#1077#1088' '#1082#1088#1080#1074#1086#1081':'
        ItemIndex = 0
        Items.Strings = (
          #1055#1077#1088#1074#1072#1103' ('#1087#1088#1103#1084#1086#1081' '#1090#1086#1082')'
          #1042#1090#1086#1088#1072#1103' ('#1086#1073#1088#1072#1090#1085#1099#1081' '#1090#1086#1082')')
        TabOrder = 1
      end
      object uiHallFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = #1055#1086#1076#1075#1086#1085#1082#1072
        TabOrder = 2
        OnClick = uiHallFeatClick
      end
      object uiHallControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 3
        OnClick = uiControlClick
      end
      object Memo3: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 4
      end
      object CurrentHall: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 5
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object uiFFTHall: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088
        TabOrder = 6
        OnClick = uiFFTHallClick
      end
      object Button6: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
        TabOrder = 7
        OnClick = Button6Click
      end
      object Lfilter2: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 8
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fd2: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp2: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz2: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '25'
        OnKeyPress = Edit1KeyPress
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
          #1055#1077#1088#1074#1072#1103' ('#1087#1088#1103#1084#1086#1081' '#1090#1086#1082')'
          #1042#1090#1086#1088#1072#1103' ('#1086#1073#1088#1072#1090#1085#1099#1081' '#1090#1086#1082')')
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
        OnClick = uiFFTFaradeyClick
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
          #1055#1077#1088#1074#1072#1103' ('#1087#1088#1103#1084#1086#1081' '#1090#1086#1082')'
          #1042#1090#1086#1088#1072#1103' ('#1086#1073#1088#1072#1090#1085#1099#1081' '#1090#1086#1082')')
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
        OnClick = uiFFTFoygtClick
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
    Top = 453
    Width = 684
    Height = 19
    Panels = <
      item
        Text = #1058#1077#1082#1091#1097#1077#1077' '#1089#1086#1089#1090#1086#1103#1085#1080#1077':'
        Width = 110
      end
      item
        Text = #1043#1086#1090#1086#1074#1072' '#1082' '#1088#1072#1073#1086#1090#1077'. '#1058#1077#1089#1090#1086#1074#1072#1103' '#1074#1077#1088#1089#1080#1103
        Width = 182
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object MainMenu1: TMainMenu
    Left = 16
    Top = 16
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
    Left = 56
    Top = 24
  end
  object OpenDialog1: TOpenDialog
    Left = 96
    Top = 24
  end
end
