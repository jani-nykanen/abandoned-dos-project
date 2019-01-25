cd Z:\home\jani\workspace\dos-experiment
wmake -f Z:\home\jani\workspace\dos-experiment\dosexperiment.mk -h -e
wcc src\app\game.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\core\core.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\core\graphics.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\core\input.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\core\eventman.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\main.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wcc src\util\mathext.c -i="C:\WATCOM/h" -w4 -e25 -zq -od -d2 -bt=dos -fo=.obj -ml
wlink name game d all sys dos op m op maxe=25 op q op symf @game.lk1
