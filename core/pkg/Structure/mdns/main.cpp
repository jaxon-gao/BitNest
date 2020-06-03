#include <mdns/mdns.h>

int
main(int argc, const char* const* argv) {
	int mode = 0;
	const char* service = "_BitNest.local.";
	const char* hostname = "dummy-host";
	int service_port = 42424;

#ifdef _WIN32

	WORD versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	if (WSAStartup(versionWanted, &wsaData)) {
		printf("Failed to initialize WinSock\n");
		return -1;
	}

	char hostname_buffer[256];
	DWORD hostname_size = (DWORD)sizeof(hostname_buffer);
	if (GetComputerNameA(hostname_buffer, &hostname_size))
		hostname = hostname_buffer;

#else

	char hostname_buffer[256];
	size_t hostname_size = sizeof(hostname_buffer);
	if (gethostname(hostname_buffer, hostname_size) == 0)
		hostname = hostname_buffer;

#endif

	for (int iarg = 0; iarg < argc; ++iarg) {
		if (strcmp(argv[iarg], "--discovery") == 0) {
			mode = 0;
		} else if (strcmp(argv[iarg], "--query") == 0) {
			mode = 1;
			++iarg;
			if (iarg < argc)
				service = argv[iarg];
		} else if (strcmp(argv[iarg], "--service") == 0) {
			mode = 2;
			++iarg;
			if (iarg < argc)
				service = argv[iarg];
		} else if (strcmp(argv[iarg], "--hostname") == 0) {
			++iarg;
			if (iarg < argc)
				hostname = argv[iarg];
		} else if (strcmp(argv[iarg], "--port") == 0) {
			++iarg;
			if (iarg < argc)
				service_port = atoi(argv[iarg]);
		}
	}

	int ret;
	if (mode == 0)
		ret = send_dns_sd();
	else if (mode == 1)
		ret = send_mdns_query(service);
	else if (mode == 2)
		ret = service_mdns(hostname, service, service_port);

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}