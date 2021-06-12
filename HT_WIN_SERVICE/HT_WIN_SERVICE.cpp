#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Service.h"

int main()
{
    WCHAR ServiceName[] = SERVICENAME;

    SERVICE_TABLE_ENTRY ServiceStartTable[] = {
        {ServiceName, ServiceMain},
        {NULL,NULL}
    };

    if (!StartServiceCtrlDispatcher(ServiceStartTable))
        trace("startServiceCtrlDispacther", std::ofstream::out);

    std::cout << "SUCCESS!!!\n";
    system("pause");
    return 0;
}
