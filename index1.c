#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_IP_LENGTH 16

// Fonction pour déterminer la classe de l'adresse IP
const char* get_ip_class(const char *ip) {
    struct in_addr addr;
    if (inet_pton(AF_INET, ip, &addr) != 1) {
        return "Adresse IP invalide";
    }
    
    unsigned char first_octet = addr.s_addr & 0xFF;
    
    if (first_octet >= 1 && first_octet <= 126) {
        return "Classe A";
    } else if (first_octet >= 128 && first_octet <= 191) {
        return "Classe B";
    } else if (first_octet >= 192 && first_octet <= 223) {
        return "Classe C";
    } else if (first_octet >= 224 && first_octet <= 239) {
        return "Classe D";
    } else if (first_octet >= 240 && first_octet <= 255) {
        return "Classe E";
    } else {
        return "Adresse IP invalide";
    }
}

int main() {
    char *data;
    char ip[MAX_IP_LENGTH];
    
    printf("Content-Type: text/html\n\n");
    
    data = getenv("QUERY_STRING");
    if (data == NULL) {
        printf("Erreur: pas de données envoyées\n");
        return 1;
    }
    
    // Extraire l'adresse IP du QUERY_STRING
    if (sscanf(data, "ip=%s", ip) != 1) {
        printf("Erreur: format de donnée invalide\n");
        return 1;
    }
    
    // Décoder les caractères spéciaux (par exemple, %20 pour les espaces)
    char decoded_ip[MAX_IP_LENGTH];
    int j = 0;
    for (int i = 0; i < strlen(ip); i++) {
        if (ip[i] == '%') {
            int value;
            sscanf(ip + i + 1, "%2x", &value);
            decoded_ip[j++] = (char)value;
            i += 2;
        } else {
            decoded_ip[j++] = ip[i];
        }
    }
    decoded_ip[j] = '\0';

    // Classer l'adresse IP
    const char *ip_class = get_ip_class(decoded_ip);
    
    printf("<html><body>");
    printf("<h1>Classe de l'adresse IP</h1>");
    printf("<p>L'adresse IP %s appartient à %s.</p>", decoded_ip, ip_class);
    printf("</body></html>");
    
    return 0;
}
