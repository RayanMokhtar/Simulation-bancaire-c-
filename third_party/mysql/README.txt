Placez ici les fichiers nécessaires au client MySQL pour la compilation et l'exécution (architecture x64, MinGW) :

1) Headers (include/)
   - third_party/mysql/include/mysql.h
   - et les en-têtes associés (mysql/*.h si fournis)

2) Bibliothèques d'import (lib/)
   - third_party/mysql/lib/libmysql.dll.a
     (ou libmariadb.dll.a si vous utilisez MariaDB Connector/C)

3) DLL runtime (bin/)
   - third_party/mysql/bin/libmysql.dll
     (ou libmariadb.dll)

Notes:
- Les fichiers doivent correspondre à votre toolchain (MinGW-w64) et à votre architecture (64-bit).
- Si vous utilisez MariaDB Connector/C (LGPL), placez ses fichiers ici, c'est compatible avec <mysql.h>.