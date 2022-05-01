#include <mictcp.h>
#include <api/mictcp_core.h>

/*
 * Permet de créer un socket entre l’application et MIC-TCP
 * Retourne le descripteur du socket ou bien -1 en cas d'erreur
 */

mic_tcp_sock sockk;
int mic_tcp_socket(start_mode sm){
   int result = -1;
   printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
   if ((result = initialize_components(sm)) == -1){
       return -1 ;
   }
   else{
        set_loss_rate(30);
        sockk.state  = CLOSED;
        sockk.fd = 1; 
        sockk.addr_locale.port = 9000;
        return sockk.fd;
    }
}   


/*
 * Permet d’attribuer une adresse à un socket.
 * Retourne 0 si succès, et -1 en cas d’échec
 */
int mic_tcp_bind(int socket, mic_tcp_sock_addr addr)
{
   printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
   return 0;
}

/*
 * Met le socket en état d'acceptation de connexions
 * Retourne 0 si succès, -1 si erreur
 */
int mic_tcp_accept(int socket, mic_tcp_sock_addr* addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    sockk.state = ESTABLISHED; 
    return 0;
}

/*
 * Permet de réclamer l’établissement d’une connexion
 * Retourne 0 si la connexion est établie, et -1 en cas d’échec
 */
int mic_tcp_connect(int socket, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    sockk.addr_distante = addr;
    sockk.state = ESTABLISHED;
    return 0;
}

/*
 * Permet de réclamer l’envoi d’une donnée applicative
 * Retourne la taille des données envoyées, et -1 en cas d'erreur
 */
int mic_tcp_send (int mic_sock, char* mesg, int mesg_size) {
    mic_tcp_pdu pdu; 
    int sent_mesg_size ;
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");    
    int num_seq ;
    if ( (sockk.state != ESTABLISHED) && (sockk.fd != mic_sock) ) {
        printf("Le socket n'est pas connecté & mauvais descripteur \n") ;
        return -1;
    } else {
            //Création du PDU 
            pdu.header.source_port = sockk.addr_locale.port;
            pdu.header.dest_port = sockk.addr_distante.port;
            pdu.header.seq_num = 0; 
            pdu.header.ack_num = 0 ; 
            pdu.header.syn = 0;
            pdu.header.ack = 0;
            pdu.header.fin = 0 ; 
            //Payload 
            pdu.payload.data = mesg;
            pdu.payload.size = mesg_size;
            //Envoi
            sent_mesg_size =  IP_send(pdu, sockk.addr_distante);
            if(sent_mesg_size ==-1){
                printf("échec de IP_send\n");
                exit(1) ;
            }
            return sent_mesg_size ;
    }

   
}

/*
 * Permet à l’application réceptrice de réclamer la récupération d’une donnée
 * stockée dans les buffers de réception du socket
 * Retourne le nombre d’octets lu ou bien -1 en cas d’erreur
 * NB : cette fonction fait appel à la fonction app_buffer_get()
 */
int mic_tcp_recv (int socket, char* mesg, int max_mesg_size)
{
    mic_tcp_payload my_payload ;
    int oct_lus ;
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    if (sockk.state != ESTABLISHED) {
        printf("Le socket n'est pas connecté.\n") ;
        return -1 ;
    }
    else{
        my_payload.data = mesg ;
        my_payload.size = max_mesg_size ;
        oct_lus = app_buffer_get(my_payload) ;
        
    }
    return oct_lus;
}

/*
 * Permet de réclamer la destruction d’un socket.
 * Engendre la fermeture de la connexion suivant le modèle de TCP.
 * Retourne 0 si tout se passe bien et -1 en cas d'erreur
 */
int mic_tcp_close (int socket)
{
    printf("[MIC-TCP] Appel de la fonction :  "); printf(__FUNCTION__); printf("\n");
    return 0;
}

/*
 * Traitement d’un PDU MIC-TCP reçu (mise à jour des numéros de séquence
 * et d'acquittement, etc.) puis insère les données utiles du PDU dans
 * le buffer de réception du socket. Cette fonction utilise la fonction
 * app_buffer_put().
 */
void process_received_PDU(mic_tcp_pdu pdu, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    app_buffer_put(pdu.payload);
}

