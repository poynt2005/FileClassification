node-gyp configure; node-gyp build; echo F | xcopy /Y .\build\Release\FileClassificator.node .\FileClassificator.node; rm .\build -r -force