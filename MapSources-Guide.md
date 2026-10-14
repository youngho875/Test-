# C4IMap XML 지도 소스

## 실행 및 설정

통합 개발 실행 파일: `D:\work\상황도\Release\Bin\C4IMap.exe`
기본 지도 정의: 실행 파일 옆의 `map-sources.xml`.

지도 우클릭 → **XML 지도 소스 설정 (래스터 / CIB / OGC)**.
목록에서 항목을 선택하고 경로/주소/축척 등을 수정한 뒤 **선택 항목 반영**, **XML 저장 및 적용**을 누릅니다. 다른 항목으로 이동하기 전에 반영 버튼을 누르세요. 별도 XML은 **XML 불러오기**로 적용할 수 있고, 외부 편집 후 **XML 다시 읽기**로 갱신합니다.
프로그램 시작 시에는 실행 파일 옆의 `map-sources.xml`을 읽습니다. 재시작에도 사용할 설정은 이 파일에 저장하세요.

기본 파일의 래스터·CIB·서버는 모두 `visual=false`로 시작합니다. 지도레이어 관리에서 사용할 항목을 체크하거나 XML의 `visual`을 `true`로 설정하세요. 현재 트리의 체크 상태는 실행 중 적용되며, 영구 보관은 설정 화면에서 XML 저장을 사용합니다.

```text
지도레이어 관리
├─ 지도 (자동 축척)  ← 기존 SHP/XML/GSS
├─ 래스터
│  ├─ 육도: 1:50,000 / 1:100,000
│  └─ 공도: 1:250,000 / 1:500,000 / 1:1,000,000 / 1:2,000,000
├─ CIB
│  └─ 위성영상 / CIB
├─ WMS   ← 통합/네트워크용
├─ WFS
└─ WMTS
```

기본 래스터 경로는 `D:/map/raster`의 축척별 폴더입니다. 실제 영상 폴더명이 `D:/map/satellite`이므로 CIB 기본값으로 사용했습니다. 요청에서 언급한 `satelite` 폴더는 확인 당시 없었습니다. 경로를 XML에서 변경할 수 있습니다.

## XML 예제

```xml
<?xml version="1.0" encoding="UTF-8"?>
<MapSources version="1">
  <Layer id="land-50k" name="1:50,000" type="Raster" family="육도">
    <visual>true</visual>
    <Path>D:/map/raster/5M</Path>
    <Extensions>tif;tiff;tl2;tc2</Extensions>
    <Recursive>true</Recursive>
    <Scale>50000</Scale>
    <MinScale>0</MinScale>
    <MaxScale>150000</MaxScale>
    <SRS></SRS>
  </Layer>
  <Layer id="cib" name="위성영상" type="CIB" family="CIB">
    <visual>false</visual>
    <Path>D:/map/satellite</Path>
    <Extensions>tif;tiff;i42;i21;i22;i41;ntf;nitf;vrt</Extensions>
    <Recursive>true</Recursive>
    <Scale>0</Scale>
    <MinScale>0</MinScale>
    <MaxScale>0</MaxScale>
  </Layer>
</MapSources>
```

- XML은 UTF-8로 저장합니다. `id`는 중복되지 않아야 합니다.
- `Path`는 폴더 또는 단일 GDAL 지원 영상/VRT 파일입니다. 상대 경로는 XML 위치 기준입니다.
- `Recursive=true`는 깊이 제한 없이 탐색합니다. 순환을 막기 위해 디렉터리 재분석 지점(정션/심볼릭 링크)은 따라가지 않습니다.
- `Extensions`는 점 없는 확장자를 세미콜론으로 구분합니다. 데이터 식별은 GDAL이 담당하므로 일반 tif 외의 tl2/tc2/i42도 사용할 수 있습니다.
- `Scale`은 대표 축척의 분모입니다. 같은 Raster/family 안에서 체크되고 범위를 만족하는 항목 중 현재 축척에 가장 가까운 항목 하나를 선택합니다.
- `MinScale`/`MaxScale`은 표시 범위의 분모이며 0은 제한 없음입니다. CIB는 범위 내 체크 항목을 표시합니다.
- 영상 자체 좌표계를 우선 사용합니다. `SRS`는 영상에 좌표계가 없는 경우의 명시적 보정값입니다. 지리참조가 없는 단순 PNG/JPG를 자동으로 지도에 배치하지는 않습니다.
- 여러 CIB 그룹/서비스는 Layer를 추가해 구성할 수 있습니다. family/type/id 등 고급 항목은 XML에서 편집합니다.

## 렌더링과 캐시

GDAL로 필요한 영역을 256×256 지도 타일로 변환하고 별도 작업 스레드에서 처리합니다. 처음 활성화한 폴더는 파일 인덱스를 작성하므로 데이터가 많으면 시간이 걸립니다. 초기 인덱스 작업은 UI 스레드를 막지 않습니다. 레이어를 껐다 켜거나 화면을 이동할 때 인덱스와 메모리 타일 캐시를 재사용합니다. 데이터가 변경되면 **타일 새로 고침** 또는 XML 다시 읽기를 사용하세요.

캐시는 메모리 방식이며 최대 256개 타일입니다. PNG/JPG 파일을 디스크에 일괄 내보내는 기능과는 별개입니다. 요청당 전체 소스의 타일 합계도 256개로 제한하고 상태줄에 제한을 알립니다. 현재 처리가 끝나기 전에는 해당 영상이 잠시 비어 보일 수 있습니다.

## WMS / WFS / WMTS

서버 주소는 아직 입력하지 않았으며 외부 서버 자동 접속도 하지 않습니다. 기본 XML의 각 서비스 항목을 선택해 입력합니다.

- WMS: `Endpoint`, `LayerName`, `Version`(1.1.1 또는 1.3.0), `Style`, `Format`. WMS 서버가 EPSG:4326을 제공해야 합니다. 1.3.0의 위도/경도 BBOX 순서를 처리합니다.
- WMTS: `Endpoint`에 GetCapabilities URL, `LayerName`, `TileMatrixSet`, `Style`을 서버의 식별자 그대로 입력합니다. TileMatrixSet 좌표계는 GDAL을 통해 화면 좌표계로 변환합니다.
- WFS: `Endpoint`, `LayerName`을 입력합니다. GDAL WFS 드라이버가 버전/스키마를 협상하고 현재 타일의 공간 범위를 조회합니다. 서버 객체는 기본 청록색으로 표시합니다. WFS 객체 선택/속성 편집 UI와 WFS-T는 이번 범위에 포함되지 않습니다. 타일당 반환 객체는 최대 10,000개로 제한하고 상태줄에 알립니다.

표시할 서비스는 `visual=true`로 설정합니다. 별도의 인증 토큰 입력 UI는 없습니다. 인증이 필요한 운영 서버는 인증 방식 확인 후 연결 설정을 보완해야 합니다.

지도 정의 XML은 mapMake에서 만드는 SHP 스타일 XML/SLD 또는 Overlay 도형 XML과 다릅니다.

## 전처리 옵션과 배포

VS2019 Release | x64 구성입니다.

| 구분 | 프로젝트 | 전처리 정의 | 실행 파일 |
|---|---|---|---|
| 통합 개발용 | C4IMap.vcxproj | 기본값 1 / 1 | 기존 Release/Bin/C4IMap.exe |
| 로컬 지도용 | C4IMap.Local.vcxproj | C4MAP_ENABLE_LOCAL=1; C4MAP_ENABLE_OGC=0 | dist/Local/C4IMap.exe |
| 네트워크 지도용 | C4IMap.Network.vcxproj | C4MAP_ENABLE_LOCAL=0; C4MAP_ENABLE_OGC=1 | dist/Network/C4IMap.exe |

프로젝트 속성 → C/C++ → 전처리기 → 전처리기 정의에서 변경합니다. 기본값은 `MapBuildOptions.h`에 있으며 명시적 프로젝트 정의가 우선합니다. 서로 다른 중간 빌드 폴더를 사용합니다.

배포할 때 `dist/Local` 또는 `dist/Network` 폴더 전체를 복사합니다. DLL, map-sources.xml, proj9/share 좌표계 자료가 필요하며 대상 PC에는 Visual C++ 2015–2022 x64 런타임이 필요합니다. 지도 원본은 배포본에 넣지 않았으므로 대상 PC 경로에 맞게 XML을 수정하세요. 전체 원본 SHP/XML/GSS 경로 설정은 기존 로컬 SHP 설정을 사용합니다.

## 검증 결과와 성능 범위

- 실제 .tl2 CADRG, .i42 CIB, GeoTIFF의 좌표 변환 및 화면 픽셀 검증 통과.
- 잘못된 XML 거부, 현재 설정 보존, visual 체크 반영 검증 통과.
- 루프백 서버에서 WMS, WMTS 이미지 및 WFS 객체 화면 표시 검증 통과. 운영 서버는 주소 미정으로 미검증.
- 통합/Local/Network 빌드 및 실제 데스크톱 설정 창의 항목 수(10/7/3) 검증 통과.
- SHP 선 LOD(최대 0.8 화면 픽셀 오차), 스타일 해석 캐시, 불일치 스타일의 좌표 변환 생략 적용. 면/기호는 유지합니다.
- SHP 표본 화면 반복 렌더링은 변경 전 190–192ms, 변경 후 188–192ms로 큰 차이가 없었습니다. LOD만으로 모든 화면의 속도 개선을 보장하지 않습니다. 실제 체감 지연의 추가 원인 측정은 별도로 필요합니다.

원본 소스 백업: `D:\work\상황도\c4imap\_backup-map-services-20261010-220856`.
