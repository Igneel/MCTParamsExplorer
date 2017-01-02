object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 792
  ClientWidth = 1076
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PG: TPageControl
    Left = -1
    Top = 1
    Width = 1074
    Height = 783
    ActivePage = TabSheet1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1043#1083#1072#1074#1085#1072#1103
      object Label1: TLabel
        Left = 16
        Top = 3
        Width = 132
        Height = 13
        Caption = #1064#1072#1075' '#1087#1086' '#1084#1072#1075#1085#1080#1090#1085#1086#1084#1091' '#1087#1086#1083#1102':'
      end
      object Label8: TLabel
        Left = 496
        Top = 515
        Width = 82
        Height = 13
        Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099':'
      end
      object Label9: TLabel
        Left = 469
        Top = 528
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label10: TLabel
        Left = 469
        Top = 559
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label2: TLabel
        Left = 752
        Top = 512
        Width = 26
        Height = 13
        Caption = #1057#1050#1054':'
      end
      object Label4: TLabel
        Left = 729
        Top = 530
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label7: TLabel
        Left = 729
        Top = 561
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label3: TLabel
        Left = 830
        Top = 515
        Width = 215
        Height = 13
        Caption = #1057#1050#1054'(%) '#1089#1095#1080#1090#1072#1077#1090#1089#1103' '#1086#1090' '#1089#1088#1077#1076#1085#1077#1075#1086' '#1079#1085#1072#1095#1077#1085#1080#1103':'
      end
      object Label5: TLabel
        Left = 897
        Top = 530
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label6: TLabel
        Left = 897
        Top = 561
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label11: TLabel
        Left = 213
        Top = 453
        Width = 49
        Height = 13
        Caption = #1064#1072#1075' '#1087#1086' T:'
      end
      object Label12: TLabel
        Left = 213
        Top = 377
        Width = 83
        Height = 13
        Caption = #1044#1083#1080#1085#1072' '#1092#1080#1083#1100#1090#1088#1072':'
      end
      object Label13: TLabel
        Left = 16
        Top = 680
        Width = 184
        Height = 13
        Caption = #1053#1072#1095#1072#1083#1100#1085#1072#1103' '#1080' '#1082#1086#1085#1077#1095#1085#1072#1103' '#1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
      end
      object eKFactor: TLabeledEdit
        Left = 288
        Top = 561
        Width = 121
        Height = 21
        EditLabel.Width = 52
        EditLabel.Height = 13
        EditLabel.Caption = 'k (1.3-1.5)'
        TabOrder = 0
        Text = '1,3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eHeavyHoleConcentration: TLabeledEdit
        Left = 289
        Top = 601
        Width = 121
        Height = 21
        EditLabel.Width = 162
        EditLabel.Height = 13
        EditLabel.Caption = #1050#1086#1085#1094#1077#1085#1090#1088#1072#1094#1080#1103' '#1090#1103#1078#1077#1083#1099#1093' '#1076#1099#1088#1086#1082':'
        TabOrder = 1
        Text = '1e22'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eAFactor: TLabeledEdit
        Left = 152
        Top = 641
        Width = 121
        Height = 21
        EditLabel.Width = 35
        EditLabel.Height = 13
        EditLabel.Caption = #1040'(5-8):'
        TabOrder = 2
        Text = '5'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eSampleThickness: TLabeledEdit
        Left = 16
        Top = 561
        Width = 97
        Height = 21
        EditLabel.Width = 93
        EditLabel.Height = 13
        EditLabel.Caption = #1058#1086#1083#1097#1080#1085#1072' '#1086#1073#1088#1072#1079#1094#1072':'
        TabOrder = 3
        Text = '1e-5'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eCurrentIntensity: TLabeledEdit
        Left = 16
        Top = 641
        Width = 97
        Height = 21
        EditLabel.Width = 74
        EditLabel.Height = 13
        EditLabel.Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072' ('#1040'):'
        TabOrder = 4
        Text = '1e-3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eTemperature: TLabeledEdit
        Left = 152
        Top = 597
        Width = 121
        Height = 21
        EditLabel.Width = 88
        EditLabel.Height = 13
        EditLabel.Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' ('#1050'):'
        TabOrder = 5
        Text = '77'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eMolarCompositionCadmium: TLabeledEdit
        Left = 152
        Top = 561
        Width = 121
        Height = 21
        EditLabel.Width = 127
        EditLabel.Height = 13
        EditLabel.Caption = #1052#1086#1083#1100#1085#1099#1081' '#1089#1086#1089#1090#1072#1074' '#1082#1072#1076#1084#1080#1103':'
        TabOrder = 6
        Text = '0,22'
        OnKeyPress = g_Nz_parKeyPress
      end
      object eCBRatio: TLabeledEdit
        Left = 16
        Top = 601
        Width = 97
        Height = 21
        EditLabel.Width = 15
        EditLabel.Height = 13
        EditLabel.Caption = 'c/b'
        TabOrder = 7
        Text = '3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object bWhiteNoiseGenerator: TButton
        Left = 15
        Top = 272
        Width = 133
        Height = 25
        Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088' '#1073#1077#1083#1086#1075#1086' '#1096#1091#1084#1072
        TabOrder = 8
        OnClick = bWhiteNoiseGeneratorClick
      end
      object bGaussianNoiseGenerator: TButton
        Left = 16
        Top = 242
        Width = 132
        Height = 25
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1096#1091#1084' '#1074' '#1089#1080#1075#1085#1072#1083
        Enabled = False
        TabOrder = 9
        OnClick = bGaussianNoiseGeneratorClick
      end
      object bCalculateCarrierParams: TButton
        Left = 16
        Top = 211
        Width = 75
        Height = 25
        Caption = #1056#1072#1089#1089#1095#1080#1090#1072#1090#1100
        TabOrder = 10
        OnClick = bCalculateCarrierParamsClick
      end
      object mDebug: TMemo
        Left = 836
        Top = 597
        Width = 209
        Height = 139
        TabOrder = 11
      end
      object BuildingPlots: TButton
        Left = 154
        Top = 17
        Width = 75
        Height = 25
        Caption = #1043#1088#1072#1092#1080#1082#1080
        Enabled = False
        TabOrder = 12
        OnClick = BuildingPlotsClick
      end
      object g_Nz_par: TStringGrid
        Left = 16
        Top = 59
        Width = 282
        Height = 137
        ColCount = 3
        DragCursor = crIBeam
        FixedCols = 0
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        TabOrder = 13
        OnKeyPress = g_Nz_parKeyPress
        ColWidths = (
          64
          132
          76)
        RowHeights = (
          24
          24
          25
          24
          24)
      end
      object g_hsize: TEdit
        Left = 16
        Top = 19
        Width = 121
        Height = 21
        TabOrder = 14
        Text = '0,01'
      end
      object Chart1: TChart
        Left = 304
        Top = 3
        Width = 335
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xx')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 15
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series1: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart2: TChart
        Left = 304
        Top = 259
        Width = 335
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xy')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 16
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series2: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart4: TChart
        Left = 645
        Top = 259
        Width = 400
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xy')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 17
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series4: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series9: TLineSeries
          Marks.Visible = False
          SeriesColor = 10708548
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart3: TChart
        Left = 645
        Top = 3
        Width = 400
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xx')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 18
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series3: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object bSaveAllPoints: TButton
        Left = 154
        Top = 211
        Width = 133
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074#1089#1077' '#1090#1086#1095#1082#1080
        TabOrder = 19
        OnClick = bSaveAllPointsClick
      end
      object Edit5: TEdit
        Left = 496
        Top = 525
        Width = 121
        Height = 21
        TabOrder = 20
        Text = '50'
        OnKeyPress = g_Nz_parKeyPress
      end
      object Edit6: TEdit
        Left = 496
        Top = 561
        Width = 121
        Height = 21
        TabOrder = 21
        Text = '1'
        OnKeyPress = g_Nz_parKeyPress
      end
      object Edit1: TEdit
        Left = 752
        Top = 531
        Width = 121
        Height = 21
        TabOrder = 22
      end
      object Edit2: TEdit
        Left = 752
        Top = 558
        Width = 121
        Height = 21
        TabOrder = 23
      end
      object Edit4: TEdit
        Left = 924
        Top = 561
        Width = 121
        Height = 21
        TabOrder = 24
      end
      object Edit3: TEdit
        Left = 924
        Top = 534
        Width = 121
        Height = 21
        TabOrder = 25
      end
      object bAutomaticCalculation: TButton
        Left = 213
        Top = 422
        Width = 74
        Height = 25
        Caption = #1056#1072#1089#1095#1077#1090
        TabOrder = 26
        OnClick = bAutomaticCalculationClick
      end
      object eStepByTemperature: TEdit
        Left = 212
        Top = 472
        Width = 75
        Height = 21
        TabOrder = 27
        Text = '10'
      end
      object bFilteringPlots: TButton
        Left = 213
        Top = 346
        Width = 75
        Height = 25
        Caption = #1060#1080#1083#1100#1090#1088
        Enabled = False
        TabOrder = 28
        OnClick = bFilteringPlotsClick
      end
      object bSaveElevenPoints: TButton
        Left = 154
        Top = 242
        Width = 133
        Height = 24
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' 11 '#1090#1086#1095#1077#1082
        TabOrder = 29
        OnClick = bSaveElevenPointsClick
      end
      object gbSavingPlot: TGroupBox
        Left = 15
        Top = 315
        Width = 185
        Height = 199
        Caption = #1057#1086#1093#1088#1072#1085#1103#1077#1084#1099#1081' '#1075#1088#1072#1092#1080#1082':'
        TabOrder = 30
        object rbFilteredUPlot: TRadioButton
          Left = 16
          Top = 109
          Width = 137
          Height = 17
          Caption = #1060#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1081' '#1089#1080#1075#1085#1072#1083
          TabOrder = 0
        end
        object rbIdealUPlot: TRadioButton
          Left = 16
          Top = 22
          Width = 113
          Height = 17
          Caption = #1063#1080#1089#1090#1099#1081' '#1089#1080#1075#1085#1072#1083
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object rbIdealTenzorPlot: TRadioButton
          Left = 16
          Top = 45
          Width = 113
          Height = 17
          Caption = #1063#1080#1089#1090#1099#1081' '#1090#1077#1085#1079#1086#1088
          TabOrder = 2
        end
        object rbFilteredTenzor: TRadioButton
          Left = 16
          Top = 132
          Width = 137
          Height = 17
          Caption = #1060#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1081' '#1090#1077#1085#1079#1086#1088
          TabOrder = 3
        end
        object rbExtrapolatedU: TRadioButton
          Left = 16
          Top = 153
          Width = 169
          Height = 17
          Caption = #1069#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1081' '#1089#1080#1075#1085#1072#1083
          TabOrder = 4
        end
        object rbExtrapolatedTenzor: TRadioButton
          Left = 16
          Top = 176
          Width = 169
          Height = 17
          Caption = #1069#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1081' '#1090#1077#1085#1079#1086#1088
          TabOrder = 5
        end
        object rbNoisyU: TRadioButton
          Left = 16
          Top = 68
          Width = 169
          Height = 17
          Caption = #1047#1072#1096#1091#1084#1083#1077#1085#1085#1099#1081' '#1089#1080#1075#1085#1072#1083
          TabOrder = 6
        end
        object rbNoisyTenzor: TRadioButton
          Left = 16
          Top = 91
          Width = 169
          Height = 17
          Caption = #1047#1072#1096#1091#1084#1083#1077#1085#1085#1099#1081' '#1090#1077#1085#1079#1086#1088
          TabOrder = 7
        end
      end
      object bLoadingPlots: TButton
        Left = 213
        Top = 315
        Width = 75
        Height = 25
        Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
        TabOrder = 31
        OnClick = bLoadingPlotsClick
      end
      object eFilterLength: TEdit
        Left = 213
        Top = 395
        Width = 75
        Height = 21
        TabOrder = 32
        Text = '25'
      end
      object bSaveFilmParams: TButton
        Left = 154
        Top = 272
        Width = 133
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099
        TabOrder = 33
        OnClick = bSaveFilmParamsClick
      end
      object bTestingSomething: TButton
        Left = 729
        Top = 727
        Width = 75
        Height = 25
        Caption = 'Test'
        TabOrder = 34
        OnClick = bTestingSomethingClick
      end
      object gbExtrapolatingMode: TGroupBox
        Left = 632
        Top = 597
        Width = 185
        Height = 105
        Caption = 'gbExtrapolatingMode'
        TabOrder = 35
        object rbFilteredOnly: TRadioButton
          Left = 24
          Top = 32
          Width = 113
          Height = 17
          Caption = 'rbFilteredOnly'
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object rbFilteredNoisyExtrapolated: TRadioButton
          Left = 24
          Top = 64
          Width = 113
          Height = 17
          Caption = 'rbFilteredNoisyExtrapolated'
          TabOrder = 1
        end
      end
      object GroupBox1: TGroupBox
        Left = 279
        Top = 628
        Width = 139
        Height = 93
        Caption = #1042#1080#1076' '#1076#1086#1073#1072#1074#1083#1103#1077#1084#1086#1075#1086' '#1096#1091#1084#1072
        TabOrder = 36
        object RadioButton1: TRadioButton
          Left = 16
          Top = 24
          Width = 113
          Height = 17
          Caption = #1053#1086#1088#1084#1072#1083#1100#1085#1099#1081' '#1096#1091#1084
          TabOrder = 0
        end
        object RadioButton2: TRadioButton
          Left = 16
          Top = 48
          Width = 113
          Height = 17
          Caption = #1044#1080#1089#1082#1088#1077#1090#1085#1099#1081' '#1096#1091#1084
          Checked = True
          TabOrder = 1
          TabStop = True
        end
      end
      object LabeledEdit1: TLabeledEdit
        Left = 440
        Top = 627
        Width = 121
        Height = 21
        EditLabel.Width = 162
        EditLabel.Height = 13
        EditLabel.Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1099#1081' '#1079#1085#1072#1095#1072#1097#1080#1081' '#1088#1072#1079#1088#1103#1076
        TabOrder = 37
        Text = '0,01'
      end
      object LabeledEdit2: TLabeledEdit
        Left = 440
        Top = 674
        Width = 121
        Height = 21
        EditLabel.Width = 109
        EditLabel.Height = 13
        EditLabel.Caption = #1042#1077#1083#1080#1095#1080#1085#1072' '#1096#1091#1084#1072' '#1074' '#1052#1047#1056
        TabOrder = 38
        Text = '2'
      end
      object Edit7: TEdit
        Left = 16
        Top = 696
        Width = 75
        Height = 21
        TabOrder = 39
        Text = '77'
      end
      object Edit8: TEdit
        Left = 128
        Top = 696
        Width = 72
        Height = 21
        TabOrder = 40
        Text = '300'
      end
      object Button1: TButton
        Left = 213
        Top = 499
        Width = 75
        Height = 25
        Caption = #1056#1072#1089#1095#1077#1090' '#1044#1080#1089#1082#1088
        TabOrder = 41
        OnClick = Button1Click
      end
    end
    object Графики: TTabSheet
      Caption = #1043#1088#1072#1092#1080#1082#1080
      ImageIndex = 1
      object Chart5: TChart
        Left = 3
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'sxx')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsSxx: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseSxx: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsSxx: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsSxx: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart6: TChart
        Left = 346
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          's_eff')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsS_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseS_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsS_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsS_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart7: TChart
        Left = 689
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Us')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 2
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsUs: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart8: TChart
        Left = 3
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'sxy')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 3
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsSxy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseSxy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsSxy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsSxy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart9: TChart
        Left = 346
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Rh_eff')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 4
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsRh_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseRh_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsRh_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsRh_eff: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart10: TChart
        Left = 689
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Uy')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        TabOrder = 5
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object gSeriesIdealParamsUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesParamsWithNoiseUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesExtrapolatedParamsUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 1330417
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object gSeriesFilteredParamsUy: TLineSeries
          Marks.Visible = False
          Brush.BackColor = clDefault
          LinePen.Color = 11048782
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object sg1: TSaveDialog
    Left = 168
    Top = 720
  end
end
