#ifndef MagneticFieldDependenceH
#define MagneticFieldDependenceH

#include <vector>
#include <algorithm>
typedef long double MyDataType;

class MagneticFieldDependence
{
public:
MagneticFieldDependence();
~MagneticFieldDependence();

//(���������, ����������, �������������, ����������/����������, ������� � �.�.,
//    ���������� ��������, ���������� �����������)

// 
std::vector<MyDataType> const &  getData();

bool saveData();
enum DependenceType {HALL_EFFECT, MAGNETORESISTANCE};
enum FeatType {ODD_FEAT, EVEN_FEAT};
enum DataKind {CURRENT_DATA, FILTERED_DATA, EXTRAPOLATED_DATA, ORIGINAL_DATA};

private:

void extrapolateData();
// �������������, �����������, ���������
void filterData(MyDataType SamplingFrequecy, MyDataType BandwidthFrequency,
     MyDataType AttenuationFrequency,int filterLength);
void multiplyB(DataKind dataKind);
void cutData();
void plotData();

inline void ReplaceDotsToComma(std::string &in, std::string & out);
inline void ReplaceCommaToDots(std::string &in, std::string & out);
bool loadData();
void featData(DataKind dataKind, long index, FeatType featType);
void averagingData(); // ���������� ������������.

MyDataType Current; // ��� �� �������, � �������.
// ������� ��������� ���� � ������ �����/��������������������,
// ����� ��������� �������������� (�������, ���������� � �.�.).
std::vector<MyDataType> B;
std::vector<MyDataType> Dependence;
// �������������� ��������, ���������� �� ���.
std::vector<MyDataType> OriginalB;
std::vector<MyDataType> OriginalDependence;
// ������������� ��������.
std::vector<MyDataType> FilteredB;
std::vector<MyDataType> FilteredDependence;
// ������������������ ��������.
std::vector<MyDataType> ExtrapolatedB;
std::vector<MyDataType> ExtrapolatedDependence;

MyDataType h; // ��� �� ���������� ����, ���� �� ������ �����������:)

unsigned int NumberOfPoints;

};

#endif