./boxes add static 2026-08-18 "Temperature=32"
./boxes add dynamic 2026-08-18 "Temperature=32"

./boxes get static 2026-08-18
./boxes get dynamic 2026-08-18

./boxes range static 2026-08-01 2026-08-18
./boxes range dynamic 2026-08-01 2026-08-18

./boxes list static
./boxes list dynamic

./boxes clear static
./boxes clear dynamic

./boxes demo
