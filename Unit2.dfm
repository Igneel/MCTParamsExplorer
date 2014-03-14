object SaveForm: TSaveForm
  Left = 227
  Top = 171
  BorderStyle = bsDialog
  Caption = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1088#1077#1079#1091#1083#1100#1090#1072#1090#1086#1074
  ClientHeight = 445
  ClientWidth = 628
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 176
    Top = 88
    Width = 99
    Height = 13
    Caption = #1047#1072#1076#1072#1085#1085#1099#1081' '#1096#1072#1075' '#1087#1086' '#1093':'
  end
  object Label2: TLabel
    Left = 16
    Top = 88
    Width = 113
    Height = 13
    Caption = #1057#1086#1093#1088#1072#1085#1103#1077#1084#1099#1081' '#1075#1088#1072#1092#1080#1082':'
  end
  object Label3: TLabel
    Left = 496
    Top = 32
    Width = 93
    Height = 26
    Alignment = taCenter
    Caption = #1057#1087#1080#1089#1086#1082' '#1090#1086#1095#1077#1082' '#1087#1086' '#1084#1072#1075#1085#1080#1090#1085#1086#1084#1091' '#1087#1086#1083#1102':'
    WordWrap = True
  end
  object SaveBut: TButton
    Left = 96
    Top = 416
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 0
    OnClick = SaveButClick
  end
  object Prepare: TRadioGroup
    Left = 16
    Top = 16
    Width = 225
    Height = 65
    Caption = #1057#1087#1086#1089#1086#1073' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1103':'
    ItemIndex = 1
    Items.Strings = (
      #1054#1087#1080#1089#1072#1085#1080#1077' '#1080' '#1076#1072#1085#1085#1099#1077' '#1074' '#1086#1076#1080#1085' '#1092#1072#1081#1083
      #1054#1087#1080#1089#1072#1085#1080#1077' '#1080' '#1076#1072#1085#1085#1099#1077' '#1074' '#1088#1072#1079#1085#1099#1077' '#1092#1072#1081#1083#1099
      #1041#1077#1079' '#1086#1087#1080#1089#1072#1085#1080#1103)
    TabOrder = 1
  end
  object Results: TRadioGroup
    Left = 248
    Top = 16
    Width = 161
    Height = 65
    Caption = #1057#1086#1093#1088#1072#1085#1103#1077#1084#1099#1077' '#1088#1077#1079#1091#1083#1100#1090#1072#1090#1099':'
    ItemIndex = 1
    Items.Strings = (
      #1042#1099#1073#1088#1072#1085#1085#1099#1077' '#1075#1088#1072#1092#1080#1082#1080
      #1054#1076#1080#1085' '#1075#1088#1072#1092#1080#1082)
    TabOrder = 2
    OnClick = ResultsClick
  end
  object SaveMode: TRadioGroup
    Left = 16
    Top = 224
    Width = 273
    Height = 89
    Caption = #1056#1077#1078#1080#1084' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1103':'
    ItemIndex = 3
    Items.Strings = (
      #1047#1072#1076#1072#1085#1085#1099#1081' '#1096#1072#1075' '#1087#1086' '#1093
      #1047#1072#1076#1072#1085#1085#1099#1081' '#1096#1072#1075' '#1087#1086' '#1091
      #1047#1072#1076#1072#1085#1085#1099#1077' '#1090#1086#1095#1082#1080
      #1042#1089#1077' '#1090#1086#1095#1082#1080)
    TabOrder = 3
    OnClick = SaveModeClick
  end
  object Memo1: TMemo
    Left = 16
    Top = 320
    Width = 321
    Height = 89
    Lines.Strings = (
      #1042#1074#1077#1076#1080#1090#1077' '#1089#1102#1076#1072' '#1086#1087#1080#1089#1072#1085#1080#1077' '#1092#1072#1081#1083#1072'/'#1080#1079#1084#1077#1088#1103#1077#1084#1099#1084' '#1088#1077#1079#1091#1083#1100#1090#1072#1090#1072#1084'.'
      #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103' '#1086#1073' '#1080#1084#1077#1085#1080' '#1092#1072#1081#1083#1086#1074' '#1089' '#1088#1077#1079#1091#1083#1100#1090#1072#1090#1072#1084#1080' '#1087#1086#1084#1077#1097#1072#1077#1090#1089#1103' '
      #1089#1102#1076#1072' '#1072#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080)
    TabOrder = 5
  end
  object Edit1: TEdit
    Left = 176
    Top = 104
    Width = 121
    Height = 21
    TabOrder = 4
    Text = '0,02'
    OnKeyPress = Edit1KeyPress
  end
  object ComboBox1: TComboBox
    Left = 16
    Top = 104
    Width = 145
    Height = 21
    Hint = 
      #1042#1099#1073#1077#1088#1080#1090#1077' '#1075#1088#1072#1092#1080#1082' '#1076#1083#1103' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1103', '#1074#1085#1080#1079#1091' '#1084#1086#1078#1085#1086' '#1091#1074#1080#1076#1077#1090#1100' '#1077#1075#1086' '#1086#1087#1080#1089#1072#1085#1080#1077 +
      '. '#1042#1085#1080#1084#1072#1085#1080#1077' - '#1088#1072#1073#1086#1090#1072#1077#1090' '#1076#1083#1103' '#1086#1076#1085#1086#1075#1086' '#1075#1088#1072#1092#1080#1082#1072'.'
    Style = csDropDownList
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
    OnChange = ComboBox1Change
    Items.Strings = (
      'Series1'
      'Series2'
      'Series3'
      'Series4'
      'Series5'
      'SeriesRes1'
      'SeriesRes2'
      'SeriesFFTRes'
      'out1'
      'SeriesHall1'
      'SeriesHall2'
      'SeriesFFTHal'
      'out2'
      'SeriesFaradey1'
      'SeriesFaradey2'
      'SeriesFFTFaradey'
      'out3'
      'SeriesFoygt1'
      'SeriesFoygt2'
      'SeriesFFTFoygt'
      'out4')
  end
  object Memo2: TMemo
    Left = 16
    Top = 152
    Width = 321
    Height = 57
    Hint = #1054#1087#1080#1089#1072#1085#1080#1077' '#1074#1099#1073#1088#1072#1085#1085#1086#1075#1086' '#1075#1088#1072#1092#1080#1082#1072
    Lines.Strings = (
      'Memo2')
    ParentShowHint = False
    ShowHint = True
    TabOrder = 7
  end
  object LB1: TListBox
    Left = 344
    Top = 96
    Width = 129
    Height = 305
    Hint = 
      #1042#1099#1073#1077#1088#1080#1090#1077' '#1085#1077#1089#1082#1086#1083#1100#1082#1086' '#1075#1088#1072#1092#1080#1082#1086#1074' '#1097#1077#1083#1082#1072#1103' '#1087#1086' '#1085#1080#1084' '#1083#1077#1074#1086#1081' '#1082#1085#1086#1087#1082#1086#1081' '#1084#1099#1096#1080', '#1087#1088 +
      #1077#1076#1074#1072#1088#1080#1090#1077#1083#1100#1085#1086' '#1079#1072#1078#1072#1074' '#1082#1083#1072#1074#1080#1096#1091' Ctrl '#1085#1072' '#1082#1083#1072#1074#1080#1072#1090#1091#1088#1077
    Style = lbOwnerDrawFixed
    ItemHeight = 14
    Items.Strings = (
      'Series1'
      'Series2'
      'Series3'
      'Series4'
      'Series5'
      'SeriesRes1'
      'SeriesRes2'
      'SeriesFFTRes'
      'out1'
      'SeriesHall1'
      'SeriesHall2'
      'SeriesFFTHal'
      'out2'
      'SeriesFaradey1'
      'SeriesFaradey2'
      'SeriesFFTFaradey'
      'out3'
      'SeriesFoygt1'
      'SeriesFoygt2'
      'SeriesFFTFoygt'
      'out4')
    MultiSelect = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
  end
  object Memo3: TMemo
    Left = 488
    Top = 64
    Width = 129
    Height = 305
    Lines.Strings = (
      '-2'
      '-1'
      '0'
      '1'
      '2')
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object Button1: TButton
    Left = 488
    Top = 376
    Width = 129
    Height = 25
    Caption = #1047#1072#1087#1086#1084#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
    TabOrder = 10
    OnClick = Button1Click
  end
  object PresetPoints: TComboBox
    Left = 472
    Top = 8
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 11
    OnChange = PresetPointsChange
  end
  object Button2: TButton
    Left = 488
    Top = 408
    Width = 129
    Height = 25
    Caption = #1059#1076#1072#1083#1080#1090#1100' '#1087#1088#1077#1089#1077#1090
    TabOrder = 12
    OnClick = Button2Click
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = '.txt'
    FileName = 'data.'
    Filter = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1092#1072#1081#1083'|*.txt'
  end
end
