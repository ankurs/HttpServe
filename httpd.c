/*
 * Initial implementation of a HTTP server, this is just a test which 
 * only manages GET request for file names
 * Author: Ankur Shrivastava
 * Licence: GPLv3+
 */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/sendfile.h>
#include<fcntl.h>

#include<event2/event.h>
#include<event2/listener.h>

#define PORT_NUMBER 9090
#define BUF_LEN 1024
#define PROCESS_NUMBER 10
#define INDEX_FILE "index.html"

char * get_mime(char * ext);


int readline(int soc, char *line, int len)
{
    /*
     * Reads a line from socket, line should be terminated by \n
     * #TODO replace with a more efficient version
     */

    fprintf(stderr,"%s\n",line);
    char *ptr,c;
    int num = 0;
    ptr  = line;
    while (1)
    {
        if ( (read(soc,&c,1) == 1 ) && ( len > num))
        {
            *ptr++ = c;
            if (c == '\n')
            {
                *ptr = '\0';
                return --num;
            }
            num++;
        }
        else
        {
            if (num >= len)
            { 
                line = realloc(line,BUF_LEN * 2);
                ptr = line + num;
                len = BUF_LEN * 2;
            }
            if (errno != EINTR)
                break;
        }
    }
    return -1;
}

char * get_file(char* line)
{
    char *tmp = strcasestr(line,"GET");
    fprintf(stderr,"get_file %s\n",line);
    if (tmp)
    {
        char *lineptr;
        lineptr = tmp + 5;
        char name[40], *ptr;
        ptr = name;
        while (strncmp(lineptr," ",1))
        {
            *ptr = *lineptr;
            ptr++;
            lineptr++;
        }
        *ptr = '\0';
        strcpy(line,name);
        if (strlen(line) == 0)
        {
            return NULL;
        }
        return line;
    }
    return NULL;
}

void run(evutil_socket_t socket)
{
    int connfd = (int) socket;
    // handle HTTP request here
    // DIRTY Clean it
    char *line;
    line = (char *) malloc(BUF_LEN);
    readline(connfd,line,BUF_LEN); // read the first request line
    printf("%s", line );
    char *filename;
    filename = get_file(line); // get file to be opened
    if (filename == NULL)
    { // if / then we send the index file
        filename = malloc(sizeof(char)*strlen(INDEX_FILE));
        strcpy(filename,"index.html\0");
    }
    fprintf(stderr,"filename -> %s\n", filename);
    char *tmp = filename;
    char *mime;
    while(*tmp != '\0' && *tmp != '.')
        tmp++;
    if (*tmp == '.')
    {
        tmp++;
        mime = get_mime(tmp);
    }
    else
    {
        mime = "text/plain";
    }
    // reply
    char con_len[50];
    char *head = "HTTP/1.1 200 OK\r\n"; // OK always #TODO handle all HTTP 
    char *head2 = malloc(100);
    snprintf(head2, 100, "Content-type: %s\r\n", mime);
    fprintf(stderr,"%s\n",head2);
    write(connfd,head,strlen(head));
    write(connfd,head2,strlen(head2));

    char *page_not_found = "<html><head><title>Server</title></head><body><h1>404 Not Found</h1></body></html>"; // page not found text
    struct stat file_status;
    if (!stat(filename,&file_status))
    { // if file exists 
        int fd = open(filename,O_RDONLY); // open file
        size_t size = file_status.st_size; // get size
        sprintf(con_len,"Content-length: %i \r\n\r\n",size); // set file size
        write(connfd,con_len,strlen(con_len)); // send Content-length
        sendfile(connfd,fd,NULL,size); // send file
    }
    else
    { // send file not found
        sprintf(con_len,"Content-length: %i \r\n\r\n",strlen(page_not_found));
        write(connfd,con_len,strlen(con_len));
        write(connfd,page_not_found,strlen(page_not_found));
    }
    free(line);
    // till here
    close(connfd);
    //exit(0);
}

char * get_mime(char * ext)
{
    if (!strcmp(ext,"scm"))
        return "application/vnd.lotus-screencam";
    
    if (!strcmp(ext,"mny"))
        return "application/x-msmoney";
    
    if (!strcmp(ext,"dist"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"gv"))
        return "text/vnd.graphviz";
    
    if (!strcmp(ext,"mpkg"))
        return "application/vnd.apple.installer+xml";
    
    if (!strcmp(ext,"edm"))
        return "application/vnd.novadigm.edm";
    
    if (!strcmp(ext,"otp"))
        return "application/vnd.oasis.opendocument.presentation-template";
    
    if (!strcmp(ext,"mp4"))
        return "video/mp4";
    
    if (!strcmp(ext,"mp2"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"mp3"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"edx"))
        return "application/vnd.novadigm.edx";
    
    if (!strcmp(ext,"scq"))
        return "application/scvp-cv-request";
    
    if (!strcmp(ext,"scs"))
        return "application/scvp-cv-response";
    
    if (!strcmp(ext,"osf"))
        return "application/vnd.yamaha.openscoreformat";
    
    if (!strcmp(ext,"wmz"))
        return "application/x-ms-wmz";
    
    if (!strcmp(ext,"ots"))
        return "application/vnd.oasis.opendocument.spreadsheet-template";
    
    if (!strcmp(ext,"zmm"))
        return "application/vnd.handheld-entertainment+xml";
    
    if (!strcmp(ext,"ott"))
        return "application/vnd.oasis.opendocument.text-template";
    
    if (!strcmp(ext,"tr"))
        return "text/troff";
    
    if (!strcmp(ext,"wmv"))
        return "video/x-ms-wmv";
    
    if (!strcmp(ext,"mwf"))
        return "application/vnd.mfer";
    
    if (!strcmp(ext,"scd"))
        return "application/x-msschedule";
    
    if (!strcmp(ext,"fh5"))
        return "image/x-freehand";
    
    if (!strcmp(ext,"fh4"))
        return "image/x-freehand";
    
    if (!strcmp(ext,"wma"))
        return "audio/x-ms-wma";
    
    if (!strcmp(ext,"wmf"))
        return "application/x-msmetafile";
    
    if (!strcmp(ext,"wmd"))
        return "application/x-ms-wmd";
    
    if (!strcmp(ext,"vxml"))
        return "application/voicexml+xml";
    
    if (!strcmp(ext,"mpe"))
        return "video/mpeg";
    
    if (!strcmp(ext,"g3"))
        return "image/g3fax";
    
    if (!strcmp(ext,"mpc"))
        return "application/vnd.mophun.certificate";
    
    if (!strcmp(ext,"lrf"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"mpn"))
        return "application/vnd.mophun.application";
    
    if (!strcmp(ext,"mpm"))
        return "application/vnd.blueice.multipass";
    
    if (!strcmp(ext,"lrm"))
        return "application/vnd.ms-lrm";
    
    if (!strcmp(ext,"rl"))
        return "application/resource-lists+xml";
    
    if (!strcmp(ext,"mpt"))
        return "application/vnd.ms-project";
    
    if (!strcmp(ext,"jisp"))
        return "application/vnd.jisp";
    
    if (!strcmp(ext,"smf"))
        return "application/vnd.stardivision.math";
    
    if (!strcmp(ext,"mpy"))
        return "application/vnd.ibm.minipay";
    
    if (!strcmp(ext,"cml"))
        return "chemical/x-cml";
    
    if (!strcmp(ext,"trm"))
        return "application/x-msterminal";
    
    if (!strcmp(ext,"tra"))
        return "application/vnd.trueapp";
    
    if (!strcmp(ext,"hbci"))
        return "application/vnd.hbci";
    
    if (!strcmp(ext,"f"))
        return "text/x-fortran";
    
    if (!strcmp(ext,"cmc"))
        return "application/vnd.cosmocaller";
    
    if (!strcmp(ext,"dll"))
        return "application/x-msdownload";
    
    if (!strcmp(ext,"fhc"))
        return "image/x-freehand";
    
    if (!strcmp(ext,"dsc"))
        return "text/prs.lines.tag";
    
    if (!strcmp(ext,"pbd"))
        return "application/vnd.powerbuilder6";
    
    if (!strcmp(ext,"aas"))
        return "application/x-authorware-seg";
    
    if (!strcmp(ext,"car"))
        return "application/vnd.curl.car";
    
    if (!strcmp(ext,"pbm"))
        return "image/x-portable-bitmap";
    
    if (!strcmp(ext,"tmo"))
        return "application/vnd.tmobile-livetv";
    
    if (!strcmp(ext,"dir"))
        return "application/x-director";
    
    if (!strcmp(ext,"distz"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"mcurl"))
        return "text/vnd.curl.mcurl";
    
    if (!strcmp(ext,"odp"))
        return "application/vnd.oasis.opendocument.presentation";
    
    if (!strcmp(ext,"htm"))
        return "text/html";
    
    if (!strcmp(ext,"ods"))
        return "application/vnd.oasis.opendocument.spreadsheet";
    
    if (!strcmp(ext,"odt"))
        return "application/vnd.oasis.opendocument.text";
    
    if (!strcmp(ext,"tex"))
        return "application/x-tex";
    
    if (!strcmp(ext,"odi"))
        return "application/vnd.oasis.opendocument.image";
    
    if (!strcmp(ext,"ecelp9600"))
        return "audio/vnd.nuera.ecelp9600";
    
    if (!strcmp(ext,"pml"))
        return "application/vnd.ctc-posml";
    
    if (!strcmp(ext,"oda"))
        return "application/oda";
    
    if (!strcmp(ext,"odb"))
        return "application/vnd.oasis.opendocument.database";
    
    if (!strcmp(ext,"odc"))
        return "application/vnd.oasis.opendocument.chart";
    
    if (!strcmp(ext,"odf"))
        return "application/vnd.oasis.opendocument.formula";
    
    if (!strcmp(ext,"odg"))
        return "application/vnd.oasis.opendocument.graphics";
    
    if (!strcmp(ext,"bdf"))
        return "application/x-font-bdf";
    
    if (!strcmp(ext,"xop"))
        return "application/xop+xml";
    
    if (!strcmp(ext,"bdm"))
        return "application/vnd.syncml.dm+wbxml";
    
    if (!strcmp(ext,"rtx"))
        return "text/richtext";
    
    if (!strcmp(ext,"sus"))
        return "application/vnd.sus-calendar";
    
    if (!strcmp(ext,"u32"))
        return "application/x-authorware-bin";
    
    if (!strcmp(ext,"abw"))
        return "application/x-abiword";
    
    if (!strcmp(ext,"ggt"))
        return "application/vnd.geogebra.tool";
    
    if (!strcmp(ext,"asx"))
        return "video/x-ms-asf";
    
    if (!strcmp(ext,"ms"))
        return "text/troff";
    
    if (!strcmp(ext,"sfd-hdstx"))
        return "application/vnd.hydrostatix.sof-data";
    
    if (!strcmp(ext,"ufd"))
        return "application/vnd.ufdl";
    
    if (!strcmp(ext,"les"))
        return "application/vnd.hhe.lesson-player";
    
    if (!strcmp(ext,"rlc"))
        return "image/vnd.fujixerox.edmics-rlc";
    
    if (!strcmp(ext,"ggb"))
        return "application/vnd.geogebra.file";
    
    if (!strcmp(ext,"pclxl"))
        return "application/vnd.hp-pclxl";
    
    if (!strcmp(ext,"teacher"))
        return "application/vnd.smart.teacher";
    
    if (!strcmp(ext,"box"))
        return "application/vnd.previewsystems.box";
    
    if (!strcmp(ext,"g2w"))
        return "application/vnd.geoplan";
    
    if (!strcmp(ext,"wdb"))
        return "application/vnd.ms-works";
    
    if (!strcmp(ext,"onepkg"))
        return "application/onenote";
    
    if (!strcmp(ext,"m3u"))
        return "audio/x-mpegurl";
    
    if (!strcmp(ext,"rdz"))
        return "application/vnd.data-vision.rdz";
    
    if (!strcmp(ext,"rdf"))
        return "application/rdf+xml";
    
    if (!strcmp(ext,"dra"))
        return "audio/vnd.dra";
    
    if (!strcmp(ext,"maker"))
        return "application/vnd.framemaker";
    
    if (!strcmp(ext,"m3a"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"fbs"))
        return "image/vnd.fastbidsheet";
    
    if (!strcmp(ext,"apr"))
        return "application/vnd.lotus-approach";
    
    if (!strcmp(ext,"txf"))
        return "application/vnd.mobius.txf";
    
    if (!strcmp(ext,"zir"))
        return "application/vnd.zul";
    
    if (!strcmp(ext,"txd"))
        return "application/vnd.genomatix.tuxedo";
    
    if (!strcmp(ext,"rcprofile"))
        return "application/vnd.ipunplugged.rcprofile";
    
    if (!strcmp(ext,"apk"))
        return "application/vnd.android.package-archive";
    
    if (!strcmp(ext,"sgm"))
        return "text/sgml";
    
    if (!strcmp(ext,"sgl"))
        return "application/vnd.stardivision.writer-global";
    
    if (!strcmp(ext,"org"))
        return "application/vnd.lotus-organizer";
    
    if (!strcmp(ext,"txt"))
        return "text/plain";
    
    if (!strcmp(ext,"wbmp"))
        return "image/vnd.wap.wbmp";
    
    if (!strcmp(ext,"wsdl"))
        return "application/wsdl+xml";
    
    if (!strcmp(ext,"tcap"))
        return "application/vnd.3gpp2.tcap";
    
    if (!strcmp(ext,"c4u"))
        return "application/vnd.clonk.c4group";
    
    if (!strcmp(ext,"dna"))
        return "application/vnd.dna";
    
    if (!strcmp(ext,"twd"))
        return "application/vnd.simtech-mindmapper";
    
    if (!strcmp(ext,"lwp"))
        return "application/vnd.lotus-wordpro";
    
    if (!strcmp(ext,"chm"))
        return "application/vnd.ms-htmlhelp";
    
    if (!strcmp(ext,"c4d"))
        return "application/vnd.clonk.c4group";
    
    if (!strcmp(ext,"xfdf"))
        return "application/vnd.adobe.xfdf";
    
    if (!strcmp(ext,"c4f"))
        return "application/vnd.clonk.c4group";
    
    if (!strcmp(ext,"msl"))
        return "application/vnd.mobius.msl";
    
    if (!strcmp(ext,"rpss"))
        return "application/vnd.nokia.radio-presets";
    
    if (!strcmp(ext,"msi"))
        return "application/x-msdownload";
    
    if (!strcmp(ext,"msh"))
        return "model/mesh";
    
    if (!strcmp(ext,"setreg"))
        return "application/set-registration-initiation";
    
    if (!strcmp(ext,"msf"))
        return "application/vnd.epson.msf";
    
    if (!strcmp(ext,"pfa"))
        return "application/x-font-type1";
    
    if (!strcmp(ext,"x32"))
        return "application/x-authorware-bin";
    
    if (!strcmp(ext,"wvx"))
        return "video/x-ms-wvx";
    
    if (!strcmp(ext,"pfb"))
        return "application/x-font-type1";
    
    if (!strcmp(ext,"wml"))
        return "text/vnd.wap.wml";
    
    if (!strcmp(ext,"pfm"))
        return "application/x-font-type1";
    
    if (!strcmp(ext,"link66"))
        return "application/vnd.route66.link66+xml";
    
    if (!strcmp(ext,"xvml"))
        return "application/xv+xml";
    
    if (!strcmp(ext,"pfr"))
        return "application/font-tdpfr";
    
    if (!strcmp(ext,"jpgm"))
        return "video/jpm";
    
    if (!strcmp(ext,"wbxml"))
        return "application/vnd.wap.wbxml";
    
    if (!strcmp(ext,"pfx"))
        return "application/x-pkcs12";
    
    if (!strcmp(ext,"fh7"))
        return "image/x-freehand";
    
    if (!strcmp(ext,"me"))
        return "text/troff";
    
    if (!strcmp(ext,"flx"))
        return "text/vnd.fmi.flexstor";
    
    if (!strcmp(ext,"ma"))
        return "application/mathematica";
    
    if (!strcmp(ext,"mb"))
        return "application/mathematica";
    
    if (!strcmp(ext,"gqf"))
        return "application/vnd.grafeq";
    
    if (!strcmp(ext,"der"))
        return "application/x-x509-ca-cert";
    
    if (!strcmp(ext,"vcx"))
        return "application/vnd.vcx";
    
    if (!strcmp(ext,"flw"))
        return "application/vnd.kde.kivio";
    
    if (!strcmp(ext,"cat"))
        return "application/vnd.ms-pki.seccat";
    
    if (!strcmp(ext,"fli"))
        return "video/x-fli";
    
    if (!strcmp(ext,"flo"))
        return "application/vnd.micrografx.flo";
    
    if (!strcmp(ext,"xht"))
        return "application/xhtml+xml";
    
    if (!strcmp(ext,"cab"))
        return "application/vnd.ms-cab-compressed";
    
    if (!strcmp(ext,"deb"))
        return "application/x-debian-package";
    
    if (!strcmp(ext,"gqs"))
        return "application/vnd.grafeq";
    
    if (!strcmp(ext,"def"))
        return "text/plain";
    
    if (!strcmp(ext,"avi"))
        return "video/x-msvideo";
    
    if (!strcmp(ext,"spx"))
        return "audio/ogg";
    
    if (!strcmp(ext,"xfdl"))
        return "application/vnd.xfdl";
    
    if (!strcmp(ext,"conf"))
        return "text/plain";
    
    if (!strcmp(ext,"tar"))
        return "application/x-tar";
    
    if (!strcmp(ext,"mpg"))
        return "video/mpeg";
    
    if (!strcmp(ext,"spp"))
        return "application/scvp-vp-response";
    
    if (!strcmp(ext,"spq"))
        return "application/scvp-vp-request";
    
    if (!strcmp(ext,"dxr"))
        return "application/x-director";
    
    if (!strcmp(ext,"dxp"))
        return "application/vnd.spotfire.dxp";
    
    if (!strcmp(ext,"rif"))
        return "application/reginfo+xml";
    
    if (!strcmp(ext,"mjp2"))
        return "video/mj2";
    
    if (!strcmp(ext,"spl"))
        return "application/x-futuresplash";
    
    if (!strcmp(ext,"dxf"))
        return "image/vnd.dxf";
    
    if (!strcmp(ext,"tao"))
        return "application/vnd.tao.intent-module-archive";
    
    if (!strcmp(ext,"spf"))
        return "application/vnd.yamaha.smaf-phrase";
    
    if (!strcmp(ext,"sis"))
        return "application/vnd.symbian.install";
    
    if (!strcmp(ext,"afm"))
        return "application/x-font-type1";
    
    if (!strcmp(ext,"chrt"))
        return "application/vnd.kde.kchart";
    
    if (!strcmp(ext,"csh"))
        return "application/x-csh";
    
    if (!strcmp(ext,"sit"))
        return "application/x-stuffit";
    
    if (!strcmp(ext,"texinfo"))
        return "application/x-texinfo";
    
    if (!strcmp(ext,"wax"))
        return "audio/x-ms-wax";
    
    if (!strcmp(ext,"sig"))
        return "application/pgp-signature";
    
    if (!strcmp(ext,"dwg"))
        return "image/vnd.dwg";
    
    if (!strcmp(ext,"mj2"))
        return "video/mj2";
    
    if (!strcmp(ext,"cst"))
        return "application/x-director";
    
    if (!strcmp(ext,"xhtml"))
        return "application/xhtml+xml";
    
    if (!strcmp(ext,"csv"))
        return "text/csv";
    
    if (!strcmp(ext,"afp"))
        return "application/vnd.ibm.modcap";
    
    if (!strcmp(ext,"csp"))
        return "application/vnd.commonspace";
    
    if (!strcmp(ext,"css"))
        return "text/css";
    
    if (!strcmp(ext,"wmls"))
        return "text/vnd.wap.wmlscript";
    
    if (!strcmp(ext,"snf"))
        return "application/x-font-snf";
    
    if (!strcmp(ext,"pqa"))
        return "application/vnd.palm";
    
    if (!strcmp(ext,"snd"))
        return "audio/basic";
    
    if (!strcmp(ext,"spot"))
        return "text/vnd.in3d.spot";
    
    if (!strcmp(ext,"spc"))
        return "application/x-pkcs7-certificates";
    
    if (!strcmp(ext,"mesh"))
        return "model/mesh";
    
    if (!strcmp(ext,"npx"))
        return "image/vnd.net-fpx";
    
    if (!strcmp(ext,"mag"))
        return "application/vnd.ecowin.chart";
    
    if (!strcmp(ext,"fh"))
        return "image/x-freehand";
    
    if (!strcmp(ext,"fm"))
        return "application/vnd.framemaker";
    
    if (!strcmp(ext,"man"))
        return "text/troff";
    
    if (!strcmp(ext,"lha"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"st"))
        return "application/vnd.sailingtracker.track";
    
    if (!strcmp(ext,"ghf"))
        return "application/vnd.groove-help";
    
    if (!strcmp(ext,"sh"))
        return "application/x-sh";
    
    if (!strcmp(ext,"so"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"sc"))
        return "application/vnd.ibm.secure-container";
    
    if (!strcmp(ext,"xhvml"))
        return "application/xv+xml";
    
    if (!strcmp(ext,"mpp"))
        return "application/vnd.ms-project";
    
    if (!strcmp(ext,"f90"))
        return "text/x-fortran";
    
    if (!strcmp(ext,"jpeg"))
        return "image/jpeg";
    
    if (!strcmp(ext,"flv"))
        return "video/x-flv";
    
    if (!strcmp(ext,"list3820"))
        return "application/vnd.ibm.modcap";
    
    if (!strcmp(ext,"ttc"))
        return "application/x-font-ttf";
    
    if (!strcmp(ext,"ttf"))
        return "application/x-font-ttf";
    
    if (!strcmp(ext,"itp"))
        return "application/vnd.shana.informed.formtemplate";
    
    if (!strcmp(ext,"joda"))
        return "application/vnd.joost.joda-archive";
    
    if (!strcmp(ext,"pcx"))
        return "image/x-pcx";
    
    if (!strcmp(ext,"h263"))
        return "video/h263";
    
    if (!strcmp(ext,"djv"))
        return "image/vnd.djvu";
    
    if (!strcmp(ext,"wrl"))
        return "model/vrml";
    
    if (!strcmp(ext,"mbk"))
        return "application/vnd.mobius.mbk";
    
    if (!strcmp(ext,"cla"))
        return "application/vnd.claymore";
    
    if (!strcmp(ext,"pct"))
        return "image/x-pict";
    
    if (!strcmp(ext,"wri"))
        return "application/x-mswrite";
    
    if (!strcmp(ext,"imp"))
        return "application/vnd.accpac.simply.imp";
    
    if (!strcmp(ext,"ims"))
        return "application/vnd.ms-ims";
    
    if (!strcmp(ext,"vtu"))
        return "model/vnd.vtu";
    
    if (!strcmp(ext,"fig"))
        return "application/x-xfig";
    
    if (!strcmp(ext,"pcl"))
        return "application/vnd.hp-pcl";
    
    if (!strcmp(ext,"gtw"))
        return "model/vnd.gtw";
    
    if (!strcmp(ext,"tsv"))
        return "text/tab-separated-values";
    
    if (!strcmp(ext,"dwf"))
        return "model/vnd.dwf";
    
    if (!strcmp(ext,"silo"))
        return "model/mesh";
    
    if (!strcmp(ext,"pcf"))
        return "application/x-font-pcf";
    
    if (!strcmp(ext,"pptx"))
        return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    
    if (!strcmp(ext,"mp2a"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"daf"))
        return "application/vnd.mobius.daf";
    
    if (!strcmp(ext,"cxx"))
        return "text/x-c";
    
    if (!strcmp(ext,"mp4a"))
        return "audio/mp4";
    
    if (!strcmp(ext,"mseed"))
        return "application/vnd.fdsn.mseed";
    
    if (!strcmp(ext,"mxs"))
        return "application/vnd.triscape.mxs";
    
    if (!strcmp(ext,"p10"))
        return "application/pkcs10";
    
    if (!strcmp(ext,"lzh"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"mxl"))
        return "application/vnd.recordare.musicxml";
    
    if (!strcmp(ext,"cer"))
        return "application/pkix-cert";
    
    if (!strcmp(ext,"docm"))
        return "application/vnd.ms-word.document.macroenabled.12";
    
    if (!strcmp(ext,"mxf"))
        return "application/mxf";
    
    if (!strcmp(ext,"qt"))
        return "video/quicktime";
    
    if (!strcmp(ext,"hqx"))
        return "application/mac-binhex40";
    
    if (!strcmp(ext,"clkx"))
        return "application/vnd.crick.clicker";
    
    if (!strcmp(ext,"xlm"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"kpt"))
        return "application/vnd.kde.kpresenter";
    
    if (!strcmp(ext,"f4v"))
        return "video/x-f4v";
    
    if (!strcmp(ext,"xlc"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"xla"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"fpx"))
        return "image/vnd.fpx";
    
    if (!strcmp(ext,"ifb"))
        return "text/calendar";
    
    if (!strcmp(ext,"h"))
        return "text/x-c";
    
    if (!strcmp(ext,"xlw"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"ifm"))
        return "application/vnd.shana.informed.formdata";
    
    if (!strcmp(ext,"xlt"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"xls"))
        return "application/vnd.ms-excel";
    
    if (!strcmp(ext,"atomcat"))
        return "application/atomcat+xml";
    
    if (!strcmp(ext,"mdi"))
        return "image/vnd.ms-modi";
    
    if (!strcmp(ext,"p"))
        return "text/x-pascal";
    
    if (!strcmp(ext,"mdb"))
        return "application/x-msaccess";
    
    if (!strcmp(ext,"list"))
        return "text/plain";
    
    if (!strcmp(ext,"hlp"))
        return "application/winhlp";
    
    if (!strcmp(ext,"gtar"))
        return "application/x-gtar";
    
    if (!strcmp(ext,"ez"))
        return "application/andrew-inset";
    
    if (!strcmp(ext,"mxml"))
        return "application/xv+xml";
    
    if (!strcmp(ext,"dic"))
        return "text/x-c";
    
    if (!strcmp(ext,"vcd"))
        return "application/x-cdlink";
    
    if (!strcmp(ext,"rq"))
        return "application/sparql-query";
    
    if (!strcmp(ext,"vcf"))
        return "text/x-vcard";
    
    if (!strcmp(ext,"rs"))
        return "application/rls-services+xml";
    
    if (!strcmp(ext,"res"))
        return "application/x-dtbresource+xml";
    
    if (!strcmp(ext,"rep"))
        return "application/vnd.businessobjects";
    
    if (!strcmp(ext,"cww"))
        return "application/prs.cww";
    
    if (!strcmp(ext,"ftc"))
        return "application/vnd.fluxtime.clip";
    
    if (!strcmp(ext,"vcs"))
        return "text/x-vcalendar";
    
    if (!strcmp(ext,"ra"))
        return "audio/x-pn-realaudio";
    
    if (!strcmp(ext,"smi"))
        return "application/smil+xml";
    
    if (!strcmp(ext,"3gp"))
        return "video/3gpp";
    
    if (!strcmp(ext,"fti"))
        return "application/vnd.anser-web-funds-transfer-initiation";
    
    if (!strcmp(ext,"rm"))
        return "application/vnd.rn-realmedia";
    
    if (!strcmp(ext,"m2a"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"dataless"))
        return "application/vnd.fdsn.seed";
    
    if (!strcmp(ext,"crd"))
        return "application/x-mscardfile";
    
    if (!strcmp(ext,"rld"))
        return "application/resource-lists-diff+xml";
    
    if (!strcmp(ext,"cmx"))
        return "image/x-cmx";
    
    if (!strcmp(ext,"pub"))
        return "application/x-mspublisher";
    
    if (!strcmp(ext,"mmr"))
        return "image/vnd.fujixerox.edmics-mmr";
    
    if (!strcmp(ext,"asc"))
        return "application/pgp-signature";
    
    if (!strcmp(ext,"mime"))
        return "message/rfc822";
    
    if (!strcmp(ext,"asf"))
        return "video/x-ms-asf";
    
    if (!strcmp(ext,"mmd"))
        return "application/vnd.chipnuts.karaoke-mmd";
    
    if (!strcmp(ext,"mmf"))
        return "application/vnd.smaf";
    
    if (!strcmp(ext,"aso"))
        return "application/vnd.accpac.simply.aso";
    
    if (!strcmp(ext,"asm"))
        return "text/x-asm";
    
    if (!strcmp(ext,"log"))
        return "text/plain";
    
    if (!strcmp(ext,"java"))
        return "text/x-java-source";
    
    if (!strcmp(ext,"pkipath"))
        return "application/pkix-pkipath";
    
    if (!strcmp(ext,"atc"))
        return "application/vnd.acucorp";
    
    if (!strcmp(ext,"midi"))
        return "audio/midi";
    
    if (!strcmp(ext,"potx"))
        return "application/vnd.openxmlformats-officedocument.presentationml.template";
    
    if (!strcmp(ext,"mvb"))
        return "application/x-msmediaview";
    
    if (!strcmp(ext,"iso"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"ltf"))
        return "application/vnd.frogans.ltf";
    
    if (!strcmp(ext,"xul"))
        return "application/vnd.mozilla.xul+xml";
    
    if (!strcmp(ext,"cdkey"))
        return "application/vnd.mediastation.cdkey";
    
    if (!strcmp(ext,"atx"))
        return "application/vnd.antix.game-component";
    
    if (!strcmp(ext,"xo"))
        return "application/vnd.olpc-sugar";
    
    if (!strcmp(ext,"dcurl"))
        return "text/vnd.curl.dcurl";
    
    if (!strcmp(ext,"pgn"))
        return "application/x-chess-pgn";
    
    if (!strcmp(ext,"pgm"))
        return "image/x-portable-graymap";
    
    if (!strcmp(ext,"htke"))
        return "application/vnd.kenameaapp";
    
    if (!strcmp(ext,"zaz"))
        return "application/vnd.zzazz.deck+xml";
    
    if (!strcmp(ext,"pgp"))
        return "application/pgp-encrypted";
    
    if (!strcmp(ext,"kfo"))
        return "application/vnd.kde.kformula";
    
    if (!strcmp(ext,"wqd"))
        return "application/vnd.wqd";
    
    if (!strcmp(ext,"gph"))
        return "application/vnd.flographit";
    
    if (!strcmp(ext,"xif"))
        return "image/vnd.xiff";
    
    if (!strcmp(ext,"kon"))
        return "application/vnd.kde.kontour";
    
    if (!strcmp(ext,"ptid"))
        return "application/vnd.pvi.ptid1";
    
    if (!strcmp(ext,"potm"))
        return "application/vnd.ms-powerpoint.template.macroenabled.12";
    
    if (!strcmp(ext,"svgz"))
        return "image/svg+xml";
    
    if (!strcmp(ext,"cdxml"))
        return "application/vnd.chemdraw+xml";
    
    if (!strcmp(ext,"uris"))
        return "text/uri-list";
    
    if (!strcmp(ext,"ncx"))
        return "application/x-dtbncx+xml";
    
    if (!strcmp(ext,"nbp"))
        return "application/vnd.wolfram.player";
    
    if (!strcmp(ext,"roff"))
        return "text/troff";
    
    if (!strcmp(ext,"ogx"))
        return "application/ogg";
    
    if (!strcmp(ext,"rsd"))
        return "application/rsd+xml";
    
    if (!strcmp(ext,"bcpio"))
        return "application/x-bcpio";
    
    if (!strcmp(ext,"zirz"))
        return "application/vnd.zul";
    
    if (!strcmp(ext,"onetoc2"))
        return "application/onenote";
    
    if (!strcmp(ext,"pnm"))
        return "image/x-portable-anymap";
    
    if (!strcmp(ext,"wav"))
        return "audio/x-wav";
    
    if (!strcmp(ext,"ogg"))
        return "audio/ogg";
    
    if (!strcmp(ext,"oga"))
        return "audio/ogg";
    
    if (!strcmp(ext,"png"))
        return "image/png";
    
    if (!strcmp(ext,"rss"))
        return "application/rss+xml";
    
    if (!strcmp(ext,"std"))
        return "application/vnd.sun.xml.draw.template";
    
    if (!strcmp(ext,"onetmp"))
        return "application/onenote";
    
    if (!strcmp(ext,"stf"))
        return "application/vnd.wt.stf";
    
    if (!strcmp(ext,"ktz"))
        return "application/vnd.kahootz";
    
    if (!strcmp(ext,"odft"))
        return "application/vnd.oasis.opendocument.formula-template";
    
    if (!strcmp(ext,"stl"))
        return "application/vnd.ms-pki.stl";
    
    if (!strcmp(ext,"dtb"))
        return "application/x-dtbook+xml";
    
    if (!strcmp(ext,"sti"))
        return "application/vnd.sun.xml.impress.template";
    
    if (!strcmp(ext,"ktr"))
        return "application/vnd.kahootz";
    
    if (!strcmp(ext,"stk"))
        return "application/hyperstudio";
    
    if (!strcmp(ext,"stw"))
        return "application/vnd.sun.xml.writer.template";
    
    if (!strcmp(ext,"str"))
        return "application/vnd.pg.format";
    
    if (!strcmp(ext,"dts"))
        return "audio/vnd.dts";
    
    if (!strcmp(ext,"fe_launch"))
        return "application/vnd.denovo.fcselayout-link";
    
    if (!strcmp(ext,"torrent"))
        return "application/x-bittorrent";
    
    if (!strcmp(ext,"mov"))
        return "video/quicktime";
    
    if (!strcmp(ext,"pre"))
        return "application/vnd.lotus-freelance";
    
    if (!strcmp(ext,"tfm"))
        return "application/x-tex-tfm";
    
    if (!strcmp(ext,"prf"))
        return "application/pics-rules";
    
    if (!strcmp(ext,"n-gage"))
        return "application/vnd.nokia.n-gage.symbian.install";
    
    if (!strcmp(ext,"prc"))
        return "application/x-mobipocket-ebook";
    
    if (!strcmp(ext,"ram"))
        return "audio/x-pn-realaudio";
    
    if (!strcmp(ext,"saf"))
        return "application/vnd.yamaha.smaf-audio";
    
    if (!strcmp(ext,"seed"))
        return "application/vnd.fdsn.seed";
    
    if (!strcmp(ext,"lbe"))
        return "application/vnd.llamagraphics.life-balance.exchange+xml";
    
    if (!strcmp(ext,"lbd"))
        return "application/vnd.llamagraphics.life-balance.desktop";
    
    if (!strcmp(ext,"qps"))
        return "application/vnd.publishare-delta-tree";
    
    if (!strcmp(ext,"atomsvc"))
        return "application/atomsvc+xml";
    
    if (!strcmp(ext,"mseq"))
        return "application/vnd.mseq";
    
    if (!strcmp(ext,"rmi"))
        return "audio/midi";
    
    if (!strcmp(ext,"bz2"))
        return "application/x-bzip2";
    
    if (!strcmp(ext,"3dml"))
        return "text/vnd.in3d.3dml";
    
    if (!strcmp(ext,"ngdat"))
        return "application/vnd.nokia.n-gage.data";
    
    if (!strcmp(ext,"mid"))
        return "audio/midi";
    
    if (!strcmp(ext,"mif"))
        return "application/vnd.mif";
    
    if (!strcmp(ext,"listafp"))
        return "application/vnd.ibm.modcap";
    
    if (!strcmp(ext,"stc"))
        return "application/vnd.sun.xml.calc.template";
    
    if (!strcmp(ext,"ccxml"))
        return "application/ccxml+xml";
    
    if (!strcmp(ext,"wcm"))
        return "application/vnd.ms-works";
    
    if (!strcmp(ext,"csml"))
        return "chemical/x-csml";
    
    if (!strcmp(ext,"ogv"))
        return "video/ogg";
    
    if (!strcmp(ext,"xdssc"))
        return "application/dssc+xml";
    
    if (!strcmp(ext,"deploy"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"rpst"))
        return "application/vnd.nokia.radio-preset";
    
    if (!strcmp(ext,"sfs"))
        return "application/vnd.spotfire.sfs";
    
    if (!strcmp(ext,"pyv"))
        return "video/vnd.ms-playready.media.pyv";
    
    if (!strcmp(ext,"kar"))
        return "audio/midi";
    
    if (!strcmp(ext,"mpeg"))
        return "video/mpeg";
    
    if (!strcmp(ext,"sisx"))
        return "application/vnd.symbian.install";
    
    if (!strcmp(ext,"pya"))
        return "audio/vnd.ms-playready.media.pya";
    
    if (!strcmp(ext,"bh2"))
        return "application/vnd.fujitsu.oasysprs";
    
    if (!strcmp(ext,"class"))
        return "application/java-vm";
    
    if (!strcmp(ext,"epub"))
        return "application/epub+zip";
    
    if (!strcmp(ext,"latex"))
        return "application/x-latex";
    
    if (!strcmp(ext,"exe"))
        return "application/x-msdownload";
    
    if (!strcmp(ext,"doc"))
        return "application/msword";
    
    if (!strcmp(ext,"tpl"))
        return "application/vnd.groove-tool-template";
    
    if (!strcmp(ext,"uri"))
        return "text/uri-list";
    
    if (!strcmp(ext,"ext"))
        return "application/vnd.novadigm.ext";
    
    if (!strcmp(ext,"tpt"))
        return "application/vnd.trid.tpt";
    
    if (!strcmp(ext,"efif"))
        return "application/vnd.picsel";
    
    if (!strcmp(ext,"dot"))
        return "application/msword";
    
    if (!strcmp(ext,"qbo"))
        return "application/vnd.intu.qbo";
    
    if (!strcmp(ext,"cdf"))
        return "application/x-netcdf";
    
    if (!strcmp(ext,"rtf"))
        return "application/rtf";
    
    if (!strcmp(ext,"nlu"))
        return "application/vnd.neurolanguage.nlu";
    
    if (!strcmp(ext,"text"))
        return "text/plain";
    
    if (!strcmp(ext,"sdc"))
        return "application/vnd.stardivision.calc";
    
    if (!strcmp(ext,"texi"))
        return "application/x-texinfo";
    
    if (!strcmp(ext,"pkg"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"pki"))
        return "application/pkixcmp";
    
    if (!strcmp(ext,"cdx"))
        return "chemical/x-cdx";
    
    if (!strcmp(ext,"cdy"))
        return "application/vnd.cinderella";
    
    if (!strcmp(ext,"xml"))
        return "application/xml";
    
    if (!strcmp(ext,"ksp"))
        return "application/vnd.kde.kspread";
    
    if (!strcmp(ext,"jar"))
        return "application/java-archive";
    
    if (!strcmp(ext,"jam"))
        return "application/vnd.jam";
    
    if (!strcmp(ext,"jad"))
        return "text/vnd.sun.j2me.app-descriptor";
    
    if (!strcmp(ext,"ief"))
        return "image/ief";
    
    if (!strcmp(ext,"xdm"))
        return "application/vnd.syncml.dm+xml";
    
    if (!strcmp(ext,"gex"))
        return "application/vnd.geometry-explorer";
    
    if (!strcmp(ext,"cpio"))
        return "application/x-cpio";
    
    if (!strcmp(ext,"xdw"))
        return "application/vnd.fujixerox.docuworks";
    
    if (!strcmp(ext,"123"))
        return "application/vnd.lotus-1-2-3";
    
    if (!strcmp(ext,"xdp"))
        return "application/vnd.adobe.xdp+xml";
    
    if (!strcmp(ext,"sse"))
        return "application/vnd.kodak-descriptor";
    
    if (!strcmp(ext,"mgz"))
        return "application/vnd.proteus.magazine";
    
    if (!strcmp(ext,"ssf"))
        return "application/vnd.epson.ssf";
    
    if (!strcmp(ext,"geo"))
        return "application/vnd.dynageo";
    
    if (!strcmp(ext,"dp"))
        return "application/vnd.osgi.dp";
    
    if (!strcmp(ext,"cif"))
        return "chemical/x-cif";
    
    if (!strcmp(ext,"musicxml"))
        return "application/vnd.recordare.musicxml+xml";
    
    if (!strcmp(ext,"aep"))
        return "application/vnd.audiograph";
    
    if (!strcmp(ext,"bat"))
        return "application/x-msdownload";
    
    if (!strcmp(ext,"aac"))
        return "audio/x-aac";
    
    if (!strcmp(ext,"crt"))
        return "application/x-x509-ca-cert";
    
    if (!strcmp(ext,"oxt"))
        return "application/vnd.openofficeorg.extension";
    
    if (!strcmp(ext,"eml"))
        return "message/rfc822";
    
    if (!strcmp(ext,"fdf"))
        return "application/vnd.fdf";
    
    if (!strcmp(ext,"dpg"))
        return "application/vnd.dpgraph";
    
    if (!strcmp(ext,"aam"))
        return "application/x-authorware-map";
    
    if (!strcmp(ext,"gram"))
        return "application/srgs";
    
    if (!strcmp(ext,"shf"))
        return "application/shf+xml";
    
    if (!strcmp(ext,"crl"))
        return "application/pkix-crl";
    
    if (!strcmp(ext,"rms"))
        return "application/vnd.jcp.javame.midlet-rms";
    
    if (!strcmp(ext,"rmp"))
        return "audio/x-pn-realaudio-plugin";
    
    if (!strcmp(ext,"ser"))
        return "application/java-serialized-object";
    
    if (!strcmp(ext,"dump"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"fg5"))
        return "application/vnd.fujitsu.oasysgp";
    
    if (!strcmp(ext,"frame"))
        return "application/vnd.framemaker";
    
    if (!strcmp(ext,"vis"))
        return "application/vnd.visionary";
    
    if (!strcmp(ext,"see"))
        return "application/vnd.seemail";
    
    if (!strcmp(ext,"pvb"))
        return "application/vnd.3gpp.pic-bw-var";
    
    if (!strcmp(ext,"hpgl"))
        return "application/vnd.hp-hpgl";
    
    if (!strcmp(ext,"dtd"))
        return "application/xml-dtd";
    
    if (!strcmp(ext,"susp"))
        return "application/vnd.sus-calendar";
    
    if (!strcmp(ext,"cmp"))
        return "application/vnd.yellowriver-custom-menu";
    
    if (!strcmp(ext,"et3"))
        return "application/vnd.eszigno3+xml";
    
    if (!strcmp(ext,"mus"))
        return "application/vnd.musician";
    
    if (!strcmp(ext,"irp"))
        return "application/vnd.irepository.package+xml";
    
    if (!strcmp(ext,"movie"))
        return "video/x-sgi-movie";
    
    if (!strcmp(ext,"osfpvg"))
        return "application/vnd.yamaha.openscoreformat.osfpvg+xml";
    
    if (!strcmp(ext,"xvm"))
        return "application/xv+xml";
    
    if (!strcmp(ext,"irm"))
        return "application/vnd.ibm.rights-management";
    
    if (!strcmp(ext,"otc"))
        return "application/vnd.oasis.opendocument.chart-template";
    
    if (!strcmp(ext,"wg"))
        return "application/vnd.pmi.widget";
    
    if (!strcmp(ext,"bed"))
        return "application/vnd.realvnc.bed";
    
    if (!strcmp(ext,"xslt"))
        return "application/xslt+xml";
    
    if (!strcmp(ext,"h264"))
        return "video/h264";
    
    if (!strcmp(ext,"boz"))
        return "application/x-bzip2";
    
    if (!strcmp(ext,"wm"))
        return "video/x-ms-wm";
    
    if (!strcmp(ext,"f77"))
        return "text/x-fortran";
    
    if (!strcmp(ext,"js"))
        return "application/javascript";
    
    if (!strcmp(ext,"wtb"))
        return "application/vnd.webturbo";
    
    if (!strcmp(ext,"fgd"))
        return "application/x-director";
    
    if (!strcmp(ext,"sv4crc"))
        return "application/x-sv4crc";
    
    if (!strcmp(ext,"c"))
        return "text/x-c";
    
    if (!strcmp(ext,"qfx"))
        return "application/vnd.intu.qfx";
    
    if (!strcmp(ext,"adp"))
        return "audio/adpcm";
    
    if (!strcmp(ext,"etx"))
        return "text/x-setext";
    
    if (!strcmp(ext,"xlam"))
        return "application/vnd.ms-excel.addin.macroenabled.12";
    
    if (!strcmp(ext,"s"))
        return "text/x-asm";
    
    if (!strcmp(ext,"fnc"))
        return "application/vnd.frogans.fnc";
    
    if (!strcmp(ext,"cod"))
        return "application/vnd.rim.cod";
    
    if (!strcmp(ext,"urls"))
        return "text/uri-list";
    
    if (!strcmp(ext,"pdf"))
        return "application/pdf";
    
    if (!strcmp(ext,"com"))
        return "application/x-msdownload";
    
    if (!strcmp(ext,"pdb"))
        return "application/vnd.palm";
    
    if (!strcmp(ext,"ecma"))
        return "application/ecmascript";
    
    if (!strcmp(ext,"xltm"))
        return "application/vnd.ms-excel.template.macroenabled.12";
    
    if (!strcmp(ext,"cxt"))
        return "application/x-director";
    
    if (!strcmp(ext,"msty"))
        return "application/vnd.muvee.style";
    
    if (!strcmp(ext,"cc"))
        return "text/x-c";
    
    if (!strcmp(ext,"pot"))
        return "application/vnd.ms-powerpoint";
    
    if (!strcmp(ext,"hvd"))
        return "application/vnd.yamaha.hv-dic";
    
    if (!strcmp(ext,"gtm"))
        return "application/vnd.groove-tool-message";
    
    if (!strcmp(ext,"gxt"))
        return "application/vnd.geonext";
    
    if (!strcmp(ext,"m13"))
        return "application/x-msmediaview";
    
    if (!strcmp(ext,"xltx"))
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
    
    if (!strcmp(ext,"kwt"))
        return "application/vnd.kde.kword";
    
    if (!strcmp(ext,"hvp"))
        return "application/vnd.yamaha.hv-voice";
    
    if (!strcmp(ext,"hvs"))
        return "application/vnd.yamaha.hv-script";
    
    if (!strcmp(ext,"cu"))
        return "application/cu-seeme";
    
    if (!strcmp(ext,"ppsx"))
        return "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
    
    if (!strcmp(ext,"swi"))
        return "application/vnd.aristanetworks.swi";
    
    if (!strcmp(ext,"ps"))
        return "application/postscript";
    
    if (!strcmp(ext,"mpga"))
        return "audio/mpeg";
    
    if (!strcmp(ext,"dotx"))
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
    
    if (!strcmp(ext,"ei6"))
        return "application/vnd.pg.osasli";
    
    if (!strcmp(ext,"swa"))
        return "application/x-director";
    
    if (!strcmp(ext,"xar"))
        return "application/vnd.xara";
    
    if (!strcmp(ext,"dis"))
        return "application/vnd.mobius.dis";
    
    if (!strcmp(ext,"swf"))
        return "application/x-shockwave-flash";
    
    if (!strcmp(ext,"json"))
        return "application/json";
    
    if (!strcmp(ext,"hpid"))
        return "application/vnd.hp-hpid";
    
    if (!strcmp(ext,"oth"))
        return "application/vnd.oasis.opendocument.text-web";
    
    if (!strcmp(ext,"oti"))
        return "application/vnd.oasis.opendocument.image-template";
    
    if (!strcmp(ext,"pic"))
        return "image/x-pict";
    
    if (!strcmp(ext,"otm"))
        return "application/vnd.oasis.opendocument.text-master";
    
    if (!strcmp(ext,"rp9"))
        return "application/vnd.cloanto.rp9";
    
    if (!strcmp(ext,"c4g"))
        return "application/vnd.clonk.c4group";
    
    if (!strcmp(ext,"mcd"))
        return "application/vnd.mcd";
    
    if (!strcmp(ext,"sldx"))
        return "application/vnd.openxmlformats-officedocument.presentationml.slide";
    
    if (!strcmp(ext,"otf"))
        return "application/x-font-otf";
    
    if (!strcmp(ext,"otg"))
        return "application/vnd.oasis.opendocument.graphics-template";
    
    if (!strcmp(ext,"bmp"))
        return "image/bmp";
    
    if (!strcmp(ext,"xlsm"))
        return "application/vnd.ms-excel.sheet.macroenabled.12";
    
    if (!strcmp(ext,"m1v"))
        return "video/mpeg";
    
    if (!strcmp(ext,"sldm"))
        return "application/vnd.ms-powerpoint.slide.macroenabled.12";
    
    if (!strcmp(ext,"xlsb"))
        return "application/vnd.ms-excel.sheet.binary.macroenabled.12";
    
    if (!strcmp(ext,"gac"))
        return "application/vnd.groove-account";
    
    if (!strcmp(ext,"dotm"))
        return "application/vnd.ms-word.template.macroenabled.12";
    
    if (!strcmp(ext,"slt"))
        return "application/vnd.epson.salt";
    
    if (!strcmp(ext,"clp"))
        return "application/x-msclip";
    
    if (!strcmp(ext,"air"))
        return "application/vnd.adobe.air-application-installer-package+zip";
    
    if (!strcmp(ext,"hdf"))
        return "application/x-hdf";
    
    if (!strcmp(ext,"mlp"))
        return "application/vnd.dolby.mlp";
    
    if (!strcmp(ext,"sdkd"))
        return "application/vnd.solent.sdkm+xml";
    
    if (!strcmp(ext,"mpg4"))
        return "video/mp4";
    
    if (!strcmp(ext,"rar"))
        return "application/x-rar-compressed";
    
    if (!strcmp(ext,"aif"))
        return "audio/x-aiff";
    
    if (!strcmp(ext,"mbox"))
        return "application/mbox";
    
    if (!strcmp(ext,"sdkm"))
        return "application/vnd.solent.sdkm+xml";
    
    if (!strcmp(ext,"ras"))
        return "image/x-cmu-raster";
    
    if (!strcmp(ext,"ami"))
        return "application/vnd.amiga.ami";
    
    if (!strcmp(ext,"jpm"))
        return "video/jpm";
    
    if (!strcmp(ext,"chat"))
        return "application/x-chat";
    
    if (!strcmp(ext,"3g2"))
        return "video/3gpp2";
    
    if (!strcmp(ext,"jpe"))
        return "image/jpeg";
    
    if (!strcmp(ext,"docx"))
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    
    if (!strcmp(ext,"jpg"))
        return "image/jpeg";
    
    if (!strcmp(ext,"sitx"))
        return "application/x-stuffitx";
    
    if (!strcmp(ext,"ivu"))
        return "application/vnd.immervision-ivu";
    
    if (!strcmp(ext,"ivp"))
        return "application/vnd.immervision-ivp";
    
    if (!strcmp(ext,"jpgv"))
        return "video/jpeg";
    
    if (!strcmp(ext,"vor"))
        return "application/vnd.stardivision.writer";
    
    if (!strcmp(ext,"in"))
        return "text/plain";
    
    if (!strcmp(ext,"mqy"))
        return "application/vnd.mobius.mqy";
    
    if (!strcmp(ext,"wks"))
        return "application/vnd.ms-works";
    
    if (!strcmp(ext,"vox"))
        return "application/x-authorware-bin";
    
    if (!strcmp(ext,"shar"))
        return "application/x-shar";
    
    if (!strcmp(ext,"wmx"))
        return "video/x-ms-wmx";
    
    if (!strcmp(ext,"pas"))
        return "text/x-pascal";
    
    if (!strcmp(ext,"paw"))
        return "application/vnd.pawaafile";
    
    if (!strcmp(ext,"psb"))
        return "application/vnd.3gpp.pic-bw-small";
    
    if (!strcmp(ext,"html"))
        return "text/html";
    
    if (!strcmp(ext,"wspolicy"))
        return "application/wspolicy+xml";
    
    if (!strcmp(ext,"qam"))
        return "application/vnd.epson.quickanime";
    
    if (!strcmp(ext,"gnumeric"))
        return "application/x-gnumeric";
    
    if (!strcmp(ext,"mxu"))
        return "video/vnd.mpegurl";
    
    if (!strcmp(ext,"xbap"))
        return "application/x-ms-xbap";
    
    if (!strcmp(ext,"wps"))
        return "application/vnd.ms-works";
    
    if (!strcmp(ext,"wpl"))
        return "application/vnd.ms-wpl";
    
    if (!strcmp(ext,"unityweb"))
        return "application/vnd.unity";
    
    if (!strcmp(ext,"ecelp7470"))
        return "audio/vnd.nuera.ecelp7470";
    
    if (!strcmp(ext,"wpd"))
        return "application/vnd.wordperfect";
    
    if (!strcmp(ext,"kia"))
        return "application/vnd.kidspiration";
    
    if (!strcmp(ext,"mobi"))
        return "application/x-mobipocket-ebook";
    
    if (!strcmp(ext,"karbon"))
        return "application/vnd.kde.karbon";
    
    if (!strcmp(ext,"cct"))
        return "application/x-director";
    
    if (!strcmp(ext,"kne"))
        return "application/vnd.kinar";
    
    if (!strcmp(ext,"eps"))
        return "application/postscript";
    
    if (!strcmp(ext,"gsf"))
        return "application/x-font-ghostscript";
    
    if (!strcmp(ext,"cdbcmsg"))
        return "application/vnd.contact.cmsg";
    
    if (!strcmp(ext,"wmlc"))
        return "application/vnd.wap.wmlc";
    
    if (!strcmp(ext,"knp"))
        return "application/vnd.kinar";
    
    if (!strcmp(ext,"pptm"))
        return "application/vnd.ms-powerpoint.presentation.macroenabled.12";
    
    if (!strcmp(ext,"mp4v"))
        return "video/mp4";
    
    if (!strcmp(ext,"mp4s"))
        return "application/mp4";
    
    if (!strcmp(ext,"xspf"))
        return "application/xspf+xml";
    
    if (!strcmp(ext,"aiff"))
        return "audio/x-aiff";
    
    if (!strcmp(ext,"davmount"))
        return "application/davmount+xml";
    
    if (!strcmp(ext,"mfm"))
        return "application/vnd.mfmp";
    
    if (!strcmp(ext,"aifc"))
        return "audio/x-aiff";
    
    if (!strcmp(ext,"ecelp4800"))
        return "audio/vnd.nuera.ecelp4800";
    
    if (!strcmp(ext,"tcl"))
        return "application/x-tcl";
    
    if (!strcmp(ext,"xer"))
        return "application/patch-ops-error+xml";
    
    if (!strcmp(ext,"gdl"))
        return "model/vnd.gdl";
    
    if (!strcmp(ext,"kwd"))
        return "application/vnd.kde.kword";
    
    if (!strcmp(ext,"skt"))
        return "application/vnd.koan";
    
    if (!strcmp(ext,"skp"))
        return "application/vnd.koan";
    
    if (!strcmp(ext,"g3w"))
        return "application/vnd.geospace";
    
    if (!strcmp(ext,"elc"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"dtshd"))
        return "audio/vnd.dts.hd";
    
    if (!strcmp(ext,"m4v"))
        return "video/x-m4v";
    
    if (!strcmp(ext,"m4u"))
        return "video/vnd.mpegurl";
    
    if (!strcmp(ext,"skd"))
        return "application/vnd.koan";
    
    if (!strcmp(ext,"rgb"))
        return "image/x-rgb";
    
    if (!strcmp(ext,"gmx"))
        return "application/vnd.gmx";
    
    if (!strcmp(ext,"ipk"))
        return "application/vnd.shana.informed.package";
    
    if (!strcmp(ext,"skm"))
        return "application/vnd.koan";
    
    if (!strcmp(ext,"fvt"))
        return "video/vnd.fvt";
    
    if (!strcmp(ext,"xenc"))
        return "application/xenc+xml";
    
    if (!strcmp(ext,"bin"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"ufdl"))
        return "application/vnd.ufdl";
    
    if (!strcmp(ext,"rnc"))
        return "application/relax-ng-compact-syntax";
    
    if (!strcmp(ext,"qwd"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"psf"))
        return "application/x-font-linux-psf";
    
    if (!strcmp(ext,"kpr"))
        return "application/vnd.kde.kpresenter";
    
    if (!strcmp(ext,"lostxml"))
        return "application/lost+xml";
    
    if (!strcmp(ext,"opf"))
        return "application/oebps-package+xml";
    
    if (!strcmp(ext,"ustar"))
        return "application/x-ustar";
    
    if (!strcmp(ext,"qwt"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"tif"))
        return "image/tiff";
    
    if (!strcmp(ext,"qxb"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"emma"))
        return "application/emma+xml";
    
    if (!strcmp(ext,"wbs"))
        return "application/vnd.criticaltools.wbs+xml";
    
    if (!strcmp(ext,"qxd"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"acutc"))
        return "application/vnd.acucorp";
    
    if (!strcmp(ext,"oprc"))
        return "application/vnd.palm";
    
    if (!strcmp(ext,"semf"))
        return "application/vnd.semf";
    
    if (!strcmp(ext,"bpk"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"semd"))
        return "application/vnd.semd";
    
    if (!strcmp(ext,"qxl"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"t"))
        return "text/troff";
    
    if (!strcmp(ext,"qxt"))
        return "application/vnd.quark.quarkxpress";
    
    if (!strcmp(ext,"viv"))
        return "video/vnd.vivo";
    
    if (!strcmp(ext,"xsl"))
        return "application/xml";
    
    if (!strcmp(ext,"xsm"))
        return "application/vnd.syncml+xml";
    
    if (!strcmp(ext,"scurl"))
        return "text/vnd.curl.scurl";
    
    if (!strcmp(ext,"jlt"))
        return "application/vnd.hp-jlyt";
    
    if (!strcmp(ext,"vss"))
        return "application/vnd.visio";
    
    if (!strcmp(ext,"vst"))
        return "application/vnd.visio";
    
    if (!strcmp(ext,"vsw"))
        return "application/vnd.visio";
    
    if (!strcmp(ext,"sxg"))
        return "application/vnd.sun.xml.writer.global";
    
    if (!strcmp(ext,"sema"))
        return "application/vnd.sema";
    
    if (!strcmp(ext,"vsf"))
        return "application/vnd.vsf";
    
    if (!strcmp(ext,"udeb"))
        return "application/x-debian-package";
    
    if (!strcmp(ext,"psd"))
        return "image/vnd.adobe.photoshop";
    
    if (!strcmp(ext,"umj"))
        return "application/vnd.umajin";
    
    if (!strcmp(ext,"ico"))
        return "image/x-icon";
    
    if (!strcmp(ext,"icm"))
        return "application/vnd.iccprofile";
    
    if (!strcmp(ext,"icc"))
        return "application/vnd.iccprofile";
    
    if (!strcmp(ext,"kml"))
        return "application/vnd.google-earth.kml+xml";
    
    if (!strcmp(ext,"ice"))
        return "x-conference/x-cooltalk";
    
    if (!strcmp(ext,"esf"))
        return "application/vnd.epson.esf";
    
    if (!strcmp(ext,"cgm"))
        return "image/cgm";
    
    if (!strcmp(ext,"aab"))
        return "application/x-authorware-bin";
    
    if (!strcmp(ext,"mc1"))
        return "application/vnd.medcalcdata";
    
    if (!strcmp(ext,"x3d"))
        return "application/vnd.hzn-3d-crossword";
    
    if (!strcmp(ext,"ics"))
        return "text/calendar";
    
    if (!strcmp(ext,"kmz"))
        return "application/vnd.google-earth.kmz";
    
    if (!strcmp(ext,"ddd"))
        return "application/vnd.fujixerox.ddd";
    
    if (!strcmp(ext,"jnlp"))
        return "application/x-java-jnlp-file";
    
    if (!strcmp(ext,"pls"))
        return "application/pls+xml";
    
    if (!strcmp(ext,"m3u8"))
        return "application/vnd.apple.mpegurl";
    
    if (!strcmp(ext,"grxml"))
        return "application/srgs+xml";
    
    if (!strcmp(ext,"ez2"))
        return "application/vnd.ezpix-album";
    
    if (!strcmp(ext,"ez3"))
        return "application/vnd.ezpix-package";
    
    if (!strcmp(ext,"plf"))
        return "application/vnd.pocketlearn";
    
    if (!strcmp(ext,"plc"))
        return "application/vnd.mobius.plc";
    
    if (!strcmp(ext,"plb"))
        return "application/vnd.3gpp.pic-bw-large";
    
    if (!strcmp(ext,"twds"))
        return "application/vnd.simtech-mindmapper";
    
    if (!strcmp(ext,"c4p"))
        return "application/vnd.clonk.c4group";
    
    if (!strcmp(ext,"nml"))
        return "application/vnd.enliven";
    
    if (!strcmp(ext,"iges"))
        return "model/iges";
    
    if (!strcmp(ext,"dcr"))
        return "application/x-director";
    
    if (!strcmp(ext,"bz"))
        return "application/x-bzip";
    
    if (!strcmp(ext,"acc"))
        return "application/vnd.americandynamics.acc";
    
    if (!strcmp(ext,"src"))
        return "application/x-wais-source";
    
    if (!strcmp(ext,"ppm"))
        return "image/x-portable-pixmap";
    
    if (!strcmp(ext,"es3"))
        return "application/vnd.eszigno3+xml";
    
    if (!strcmp(ext,"zip"))
        return "application/zip";
    
    if (!strcmp(ext,"ace"))
        return "application/x-ace-compressed";
    
    if (!strcmp(ext,"dd2"))
        return "application/vnd.oma.dd2+xml";
    
    if (!strcmp(ext,"obd"))
        return "application/x-msbinder";
    
    if (!strcmp(ext,"cmdf"))
        return "chemical/x-cmdf";
    
    if (!strcmp(ext,"wmlsc"))
        return "application/vnd.wap.wmlscriptc";
    
    if (!strcmp(ext,"xap"))
        return "application/x-silverlight-app";
    
    if (!strcmp(ext,"acu"))
        return "application/vnd.acucobol";
    
    if (!strcmp(ext,"srx"))
        return "application/sparql-results+xml";
    
    if (!strcmp(ext,"book"))
        return "application/vnd.framemaker";
    
    if (!strcmp(ext,"clkk"))
        return "application/vnd.crick.clicker.keyboard";
    
    if (!strcmp(ext,"ppd"))
        return "application/vnd.cups-ppd";
    
    if (!strcmp(ext,"tiff"))
        return "image/tiff";
    
    if (!strcmp(ext,"xyz"))
        return "chemical/x-xyz";
    
    if (!strcmp(ext,"pcurl"))
        return "application/vnd.curl.pcurl";
    
    if (!strcmp(ext,"cpp"))
        return "text/x-c";
    
    if (!strcmp(ext,"nsf"))
        return "application/vnd.lotus-notes";
    
    if (!strcmp(ext,"pps"))
        return "application/vnd.ms-powerpoint";
    
    if (!strcmp(ext,"ppt"))
        return "application/vnd.ms-powerpoint";
    
    if (!strcmp(ext,"clkp"))
        return "application/vnd.crick.clicker.palette";
    
    if (!strcmp(ext,"dfac"))
        return "application/vnd.dreamfactory";
    
    if (!strcmp(ext,"clkt"))
        return "application/vnd.crick.clicker.template";
    
    if (!strcmp(ext,"djvu"))
        return "image/vnd.djvu";
    
    if (!strcmp(ext,"clkw"))
        return "application/vnd.crick.clicker.wordbank";
    
    if (!strcmp(ext,"xpi"))
        return "application/x-xpinstall";
    
    if (!strcmp(ext,"w3d"))
        return "application/x-director";
    
    if (!strcmp(ext,"xpm"))
        return "image/x-xpixmap";
    
    if (!strcmp(ext,"fzs"))
        return "application/vnd.fuzzysheet";
    
    if (!strcmp(ext,"ppam"))
        return "application/vnd.ms-powerpoint.addin.macroenabled.12";
    
    if (!strcmp(ext,"wad"))
        return "application/x-doom";
    
    if (!strcmp(ext,"setpay"))
        return "application/set-payment-initiation";
    
    if (!strcmp(ext,"gif"))
        return "image/gif";
    
    if (!strcmp(ext,"xpx"))
        return "application/vnd.intercon.formnet";
    
    if (!strcmp(ext,"smil"))
        return "application/smil+xml";
    
    if (!strcmp(ext,"xps"))
        return "application/vnd.ms-xpsdocument";
    
    if (!strcmp(ext,"xpr"))
        return "application/vnd.is-xpr";
    
    if (!strcmp(ext,"gim"))
        return "application/vnd.groove-identity-message";
    
    if (!strcmp(ext,"xpw"))
        return "application/vnd.intercon.formnet";
    
    if (!strcmp(ext,"sdw"))
        return "application/vnd.stardivision.writer";
    
    if (!strcmp(ext,"sdp"))
        return "application/sdp";
    
    if (!strcmp(ext,"ipfix"))
        return "application/ipfix";
    
    if (!strcmp(ext,"sdd"))
        return "application/vnd.stardivision.impress";
    
    if (!strcmp(ext,"hh"))
        return "text/x-c";
    
    if (!strcmp(ext,"sda"))
        return "application/vnd.stardivision.draw";
    
    if (!strcmp(ext,"atom"))
        return "application/atom+xml";
    
    if (!strcmp(ext,"pwn"))
        return "application/vnd.3m.post-it-notes";
    
    if (!strcmp(ext,"mscml"))
        return "application/mediaservercontrol+xml";
    
    if (!strcmp(ext,"sv4cpio"))
        return "application/x-sv4cpio";
    
    if (!strcmp(ext,"mathml"))
        return "application/mathml+xml";
    
    if (!strcmp(ext,"m14"))
        return "application/x-msmediaview";
    
    if (!strcmp(ext,"lvp"))
        return "audio/vnd.lucent.voice";
    
    if (!strcmp(ext,"dmg"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"uu"))
        return "text/x-uuencode";
    
    if (!strcmp(ext,"mts"))
        return "model/vnd.mts";
    
    if (!strcmp(ext,"cii"))
        return "application/vnd.anser-web-certificate-issue-initiation";
    
    if (!strcmp(ext,"sgml"))
        return "text/sgml";
    
    if (!strcmp(ext,"azf"))
        return "application/vnd.airzip.filesecure.azf";
    
    if (!strcmp(ext,"cil"))
        return "application/vnd.ms-artgalry";
    
    if (!strcmp(ext,"dms"))
        return "application/octet-stream";
    
    if (!strcmp(ext,"p12"))
        return "application/x-pkcs12";
    
    if (!strcmp(ext,"svg"))
        return "image/svg+xml";
    
    if (!strcmp(ext,"utz"))
        return "application/vnd.uiq.theme";
    
    if (!strcmp(ext,"vsd"))
        return "application/vnd.visio";
    
    if (!strcmp(ext,"xwd"))
        return "image/x-xwindowdump";
    
    if (!strcmp(ext,"azs"))
        return "application/vnd.airzip.filesecure.azs";
    
    if (!strcmp(ext,"azw"))
        return "application/vnd.amazon.ebook";
    
    if (!strcmp(ext,"sxc"))
        return "application/vnd.sun.xml.calc";
    
    if (!strcmp(ext,"sxd"))
        return "application/vnd.sun.xml.draw";
    
    if (!strcmp(ext,"p7s"))
        return "application/pkcs7-signature";
    
    if (!strcmp(ext,"xlsx"))
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    
    if (!strcmp(ext,"ai"))
        return "application/postscript";
    
    if (!strcmp(ext,"sxi"))
        return "application/vnd.sun.xml.impress";
    
    if (!strcmp(ext,"nnd"))
        return "application/vnd.noblenet-directory";
    
    if (!strcmp(ext,"sxm"))
        return "application/vnd.sun.xml.math";
    
    if (!strcmp(ext,"ppsm"))
        return "application/vnd.ms-powerpoint.slideshow.macroenabled.12";
    
    if (!strcmp(ext,"fly"))
        return "text/vnd.fly";
    
    if (!strcmp(ext,"p7b"))
        return "application/x-pkcs7-certificates";
    
    if (!strcmp(ext,"p7c"))
        return "application/pkcs7-mime";
    
    if (!strcmp(ext,"aw"))
        return "application/applixware";
    
    if (!strcmp(ext,"sxw"))
        return "application/vnd.sun.xml.writer";
    
    if (!strcmp(ext,"nnw"))
        return "application/vnd.noblenet-web";
    
    if (!strcmp(ext,"curl"))
        return "text/vnd.curl";
    
    if (!strcmp(ext,"p7m"))
        return "application/pkcs7-mime";
    
    if (!strcmp(ext,"nns"))
        return "application/vnd.noblenet-sealer";
    
    if (!strcmp(ext,"uoml"))
        return "application/vnd.uoml+xml";
    
    if (!strcmp(ext,"grv"))
        return "application/vnd.groove-injector";
    
    if (!strcmp(ext,"igs"))
        return "model/iges";
    
    if (!strcmp(ext,"hps"))
        return "application/vnd.hp-hps";
    
    if (!strcmp(ext,"nb"))
        return "application/mathematica";
    
    if (!strcmp(ext,"nc"))
        return "application/x-netcdf";
    
    if (!strcmp(ext,"igx"))
        return "application/vnd.micrografx.igx";
    
    if (!strcmp(ext,"application"))
        return "application/x-ms-application";
    
    if (!strcmp(ext,"h261"))
        return "video/h261";
    
    if (!strcmp(ext,"gre"))
        return "application/vnd.geometry-explorer";
    
    if (!strcmp(ext,"oas"))
        return "application/vnd.fujitsu.oasys";
    
    if (!strcmp(ext,"fst"))
        return "image/vnd.fst";
    
    if (!strcmp(ext,"p7r"))
        return "application/x-pkcs7-certreqresp";
    
    if (!strcmp(ext,"igl"))
        return "application/vnd.igloader";
    
    if (!strcmp(ext,"vrml"))
        return "model/vrml";
    
    if (!strcmp(ext,"bmi"))
        return "application/vnd.bmi";
    
    if (!strcmp(ext,"btif"))
        return "image/prs.btif";
    
    if (!strcmp(ext,"ssml"))
        return "application/ssml+xml";
    
    if (!strcmp(ext,"au"))
        return "audio/basic";
    
    if (!strcmp(ext,"fsc"))
        return "application/vnd.fsc.weblaunch";
    
    if (!strcmp(ext,"dvi"))
        return "application/x-dvi";
    
    if (!strcmp(ext,"for"))
        return "text/x-fortran";
    
    if (!strcmp(ext,"sbml"))
        return "application/sbml+xml";
    
    if (!strcmp(ext,"svd"))
        return "application/vnd.svd";
    
    if (!strcmp(ext,"vcg"))
        return "application/vnd.groove-vcard";
    
    if (!strcmp(ext,"dssc"))
        return "application/dssc+der";
    
    if (!strcmp(ext,"portpkg"))
        return "application/vnd.macports.portpkg";
    
    if (!strcmp(ext,"eol"))
        return "audio/vnd.digital-winds";
    
    if (!strcmp(ext,"xbm"))
        return "image/x-xbitmap";
    
    if (!strcmp(ext,"iif"))
        return "application/vnd.shana.informed.interchange";
    
    if (!strcmp(ext,"oa3"))
        return "application/vnd.fujitsu.oasys3";
    
    if (!strcmp(ext,"oa2"))
        return "application/vnd.fujitsu.oasys2";
    
    if (!strcmp(ext,"eot"))
        return "application/vnd.ms-fontobject";
    
    if (!strcmp(ext,"onetoc"))
        return "application/onenote";
    
    if (!strcmp(ext,"xbd"))
        return "application/vnd.fujixerox.docuworks.binder";
    
    if (!strcmp(ext,"mrc"))
        return "application/marc";
    
    if (!strcmp(ext,"cpt"))
        return "application/mac-compactpro";
    
    if (!strcmp(ext,"m2v"))
        return "video/mpeg";    

    return NULL;
}
