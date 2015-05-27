object Form4: TForm4
  Left = 691
  Top = 405
  Width = 244
  Height = 214
  Caption = #1057#1076#1074#1080#1075' '#1075#1088#1072#1092#1080#1082#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label7: TLabel
    Left = 113
    Top = 9
    Width = 96
    Height = 13
    Caption = #1048#1085#1090#1077#1088#1074#1072#1083' '#1087#1086' '#1087#1086#1083#1102':'
  end
  object uiShiftValue: TLabeledEdit
    Left = 16
    Top = 25
    Width = 89
    Height = 21
    EditLabel.Width = 86
    EditLabel.Height = 13
    EditLabel.Caption = #1042#1077#1083#1080#1095#1080#1085#1072' '#1089#1076#1074#1080#1075#1072
    LabelPosition = lpAbove
    LabelSpacing = 3
    TabOrder = 0
    Text = '0,01'
  end
  object uiLeftBound: TComboBox
    Left = 112
    Top = 25
    Width = 49
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 1
    Text = '-2'
    Items.Strings = (
      '-2'
      '-1'
      '0'
      '1'
      '2')
  end
  object uiRightBound: TComboBox
    Left = 161
    Top = 25
    Width = 49
    Height = 21
    ItemHeight = 13
    ItemIndex = 2
    TabOrder = 2
    Text = '0'
    Items.Strings = (
      '-2'
      '-1'
      '0'
      '1'
      '2')
  end
  object uiDataKind: TRadioGroup
    Left = 16
    Top = 53
    Width = 185
    Height = 81
    Caption = #1042#1080#1076' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080':'
    ItemIndex = 0
    Items.Strings = (
      #1058#1077#1082#1091#1097#1080#1077' '#1076#1072#1085#1085#1099#1077
      #1060#1080#1083#1100#1090#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
      #1069#1082#1089#1090#1088#1072#1087#1086#1083#1080#1088#1086#1074#1072#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
      #1059#1089#1088#1077#1076#1077#1085#1085#1099#1077' '#1076#1072#1085#1085#1099#1077)
    TabOrder = 3
  end
  object bResShiftCurve: TButton
    Left = 48
    Top = 141
    Width = 121
    Height = 25
    Caption = #1057#1076#1074#1080#1085#1091#1090#1100' '#1075#1088#1072#1092#1080#1082
    TabOrder = 4
  end
end
