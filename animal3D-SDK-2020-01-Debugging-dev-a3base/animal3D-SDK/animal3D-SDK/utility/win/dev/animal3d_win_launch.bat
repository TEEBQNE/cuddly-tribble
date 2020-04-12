::Utility script to launch SDK in latest version of Visual Studio.
::By Dan Buckstein


rem VISUAL STUDIO 2019 IF PATH EXISTS
	set a3vstoolsdir="%VS160COMNTOOLS%"
	set a3vslaunchpath="%VS160COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v142
	set a3sdkversion=10.0


	start %a3vslaunchpath% "%ANIMAL3D_SDK%project\VisualStudio\_SLN\%~1\%~1.sln"

