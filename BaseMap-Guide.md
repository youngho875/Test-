# 기본지도와 축척 표시

실행 파일과 같은 폴더의 `base-map.xml`을 사용합니다.
일반 실행은 `D:\work\상황도\Release\Bin\base-map.xml`, 배포용은 각 `dist\Local` 및 `dist\Network` 폴더입니다.
프로그램을 종료하고 XML을 편집한 후 다시 실행하세요. 지도레이어관리 체크 변경은 해당 XML에 저장됩니다. 파일을 저장할 쓰기 권한이 필요합니다.

- `SmallScale`: 소축척 지도. `Path`는 경계 SHP, `LabelPath`는 지명 SHP.
- `World`: 세계지도. `Path`는 대륙, `BoundaryPath`는 국경, `LabelPath` 및 `CityPath`는 지명 SHP.
- `NaturalEarth`: 배경영상. `Path`는 기존 geographic TMS 타일 폴더. 실제 기본 경로는 `D:/webcop/NaturalEarthII`입니다.
- 각 항목의 `visual`: `true` 표시, `false` 숨김.
- `Names/visual`: 체크된 소축척·세계지도의 지명 표시 여부. 겹치는 지명은 일부 생략합니다.
- `ScaleStatus`: 상태 표시줄의 `Scale 1:분모`.
- `ScaleBar`: 화면 왼쪽 아래 거리 막대(m/km).
- `Compass`: 화면 오른쪽 위 진북 방향 표시. 현재 지도는 북쪽이 위입니다.

축척 분모는 화면 중심의 지상 거리와 Windows DPI로 계산합니다. 거리 막대는 막대 위치의 위도로 계산하므로, 넓은 지역에서는 화면 중앙 축척과 다를 수 있습니다. 모니터의 실제 물리적 크기를 별도로 보정하는 기능은 아닙니다.
기본지도는 로컬/통합 빌드에서 표시되며 네트워크 전용 빌드에서는 제외됩니다. 축척·나침반은 모든 빌드에서 사용할 수 있습니다.
래스터/CIB 및 WMS/WFS/WMTS는 기존 `map-sources.xml`을 사용합니다.
