#include <stdio.h> // for printf, fprintf, perror, snprintf, stderr
#include <stdlib.h> // for exit
#include <string.h> // for memset
#include <netdb.h> // for getaddrinfo, EAI_SYSTEM, gai_strerror, struct addrinfo, getnameinfo,
									 // freeaddrinfo
#include <arpa/inet.h> // for inet_ntop

char* get_socket_type_name_from_code(int code) {
	switch (code) { 
		case SOCK_STREAM: return "SOCK_STREAM"; // TCP
		case SOCK_DGRAM: return "SOCK_DGRAM";   // UDP
		case SOCK_RAW: return "SOCK_RAW";       // IP
		case SOCK_RDM: return "SOCK_RDM"; // Reliable datagram layer that does not guarantee ordering.
		case SOCK_SEQPACKET: return "SOCK_SEQPACKET";
		//case SOCK_CLOEXEC: return "SOCK_CLOEXEC";
		//case SOCK_NONBLOCK: return "SOCK_NONBLOCK";
		default: return "SOCK_???";
	}
}

char* get_address_family_name_from_code(uint8_t code) {
	switch (code) {
		case AF_UNSPEC: return "AF_UNSPEC";
		case AF_LOCAL: return "AF_LOCAL"; // man 7 unix
		case AF_INET: return "AF_INET";
		case AF_AX25: return "AF_AX25"; // Amateur radio AX.25 protocol
		case AF_IPX: return "AF_IPX"; // IPX - Novell protocols 
		case AF_APPLETALK: return "AF_APPLETALK"; // man 7 ddp
		case AF_NETROM: return "AF_NETROM";
		case AF_BRIDGE: return "AF_BRIDGE";
		case AF_ATMPVC: return "AF_ATMPVC"; // Access to raw ATM PVCs
		case AF_X25: return "AF_X25"; // man 7 x25
		case AF_INET6: return "AF_INET6";
		case AF_ROSE: return "AF_ROSE";
		case AF_DECnet: return "AF_DECnet";
		case AF_NETBEUI: return "AF_NETBEUI";
		case AF_SECURITY: return "AF_SECURITY";
		case AF_KEY: return "AF_KEY";
		case AF_NETLINK: return "AF_NETLINK/AF_ROUTE"; // man 7 netlink - Kernel user interface device
		case AF_PACKET: return "AF_PACKET"; // man 7 packet - Low level packet interface
		case AF_ASH: return "AF_ASH";
		case AF_ECONET: return "AF_ECONET";
		case AF_ATMSVC: return "AF_ATMSVC";
		case AF_RDS: return "AF_RDS";
		case AF_SNA: return "AF_SNA";
		case AF_IRDA: return "AF_IRDA";
		case AF_PPPOX: return "AF_PPPOX";
		case AF_WANPIPE: return "AF_WANPIPE";
		case AF_LLC: return "AF_LLC";
		case AF_IB: return "AF_IB";
		case AF_MPLS: return "AF_MPLS";
		case AF_CAN: return "AF_CAN";
		case AF_TIPC: return "AF_TIPC";
		case AF_BLUETOOTH: return "AF_BLUETOOTH";
		case AF_IUCV: return "AF_IUCV";
		case AF_RXRPC: return "AF_RXRPC";
		case AF_ISDN: return "AF_ISDN";
		case AF_PHONET: return "AF_PHONET";
		case AF_IEEE802154: return "AF_IEEE802154";
		case AF_CAIF: return "AF_CAIF";
		case AF_ALG: return "AF_ALG"; // Interface to kernel crypto API
		case AF_NFC: return "AF_NFC";
		case AF_VSOCK: return "AF_VSOCK";
		case AF_MAX: return "AF_MAX";
		default: return "AF_???";
	}
}

#define BUFSIZE 200

const char* get_address_description(struct sockaddr * address) {
  static char buffer[BUFSIZE]; // MT_UNSAFE 
	int conversion_result;
	switch (address->sa_family) {
		case AF_INET6:
			{
				struct sockaddr_in6* sockaddr_in_6;
				sockaddr_in_6 = (struct sockaddr_in6*) address;

				char ip6[INET6_ADDRSTRLEN];
				if (inet_ntop(AF_INET6, &sockaddr_in_6->sin6_addr, ip6, INET6_ADDRSTRLEN) == NULL) {
					printf("inet_ntop for AF_INET6 in get_address_description failed\n");
					perror("get_address_description");
					exit(EXIT_FAILURE);
				}

				conversion_result = snprintf(buffer, BUFSIZE, "{port: %d, flowinfo: %d, address: %s, scope id: %d}",
						sockaddr_in_6->sin6_port,
						sockaddr_in_6->sin6_flowinfo,
						ip6,
						sockaddr_in_6->sin6_scope_id);
				if (conversion_result < 0) { // an error occurred
					printf("snprintf in get_address_description failed\n");
					exit(EXIT_FAILURE);
				} else {
					return buffer;
				}
			}
		case AF_INET: 
			{
				struct sockaddr_in* sockaddr_in_4;
				sockaddr_in_4 = (struct sockaddr_in*) address;

				char ip4[INET_ADDRSTRLEN];
				if (inet_ntop(AF_INET, &sockaddr_in_4->sin_addr, ip4, INET_ADDRSTRLEN) == NULL) {
					printf("inet_ntop for AF_INET in get_address_description failed\n");
					perror("get_address_description");
					exit(EXIT_FAILURE);
				}

				int conversion_result = snprintf(buffer, BUFSIZE, "{port: %d, address: %s}",
						sockaddr_in_4->sin_port, ip4);
				if (conversion_result < 0) { // an error occurred
					printf("snprintf in get_address_description failed\n");
					exit(EXIT_FAILURE);
				} else {
					return buffer;
				}
			}
	}
}

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif

#define HOST_NAME 1

// MT-UNSAFE 
// - getprotobynumber
int main(int argc, char** argv)
{
	struct addrinfo* result;
	struct addrinfo* res;
  struct addrinfo hints;
	int error;

	if(argc<2){
		printf("Usage: dns_resolve <hostname>\n");
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;

	/* resolve the domain name into a list of addresses */
	error = getaddrinfo(argv[HOST_NAME], /*"http"*/NULL, &hints, &result);
	if (error != 0) {   
		if (error == EAI_SYSTEM) {
			perror("getaddrinfo EAI_SYSTEM");
		} else {
			fprintf(stderr, "error in getaddrinfo for hostname '%s': %s\n", argv[HOST_NAME], gai_strerror(error));
		}   
		exit(EXIT_FAILURE);
	}   

	// count number of results
	int result_count = 0;
	for (res = result; res != NULL; res = res->ai_next) {   
		result_count++;
	}   
	printf("%d results found\n", result_count);

	for (res = result; res != NULL; res = res->ai_next) {   
		printf("address information: {flags: %d, family: %s, socktype: %s, protocol: %s, addrlen: %d, sockaddr: %s, canonname: %s}\n",
				res->ai_flags,
				get_address_family_name_from_code(res->ai_family),
				get_socket_type_name_from_code(res->ai_socktype),
	      getprotobynumber(res->ai_protocol)->p_name,
				res->ai_addrlen,
        get_address_description(res->ai_addr),
				res->ai_canonname);
	}   

	/* loop over all returned results and do inverse lookup */
	for (res = result; res != NULL; res = res->ai_next) {   
		char hostname[NI_MAXHOST];
		error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
		if (error != 0) {
			fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
			continue;
		}
		if (*hostname != '\0')
			printf("hostname: %s\n", hostname);
	}   

	freeaddrinfo(result);
	return 0;
}
