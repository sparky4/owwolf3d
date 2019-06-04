// WL_MAIN.C

#include "WL_DEF.H"
#pragma hdrstop


/*
=============================================================================

                                                   WOLFENSTEIN 3-D

                                          An Id Software production

                                                   by John Carmack

=============================================================================
*/

extern byte signon[];

/*
=============================================================================

                                                 LOCAL CONSTANTS

=============================================================================
*/


#define FOCALLENGTH     (0x5700l)               // in global coordinates
#define VIEWGLOBAL      0x10000                 // globals visable flush to wall

#define VIEWWIDTH       256                     // size of view window
#define VIEWHEIGHT      144

/*
=============================================================================

                                                 GLOBAL VARIABLES

=============================================================================
*/

char            str[80],str2[20];
int             tedlevelnum;
boolean tedlevel;
boolean nospr;
boolean IsA386;
int             dirangle[9] = {0,ANGLES/8,2*ANGLES/8,3*ANGLES/8,4*ANGLES/8,
                                                                5*ANGLES/8,6*ANGLES/8,7*ANGLES/8,ANGLES};

//
// proejection variables
//
fixed                   focallength;
unsigned                screenofs;
int                     viewwidth;
int                     viewheight;
short                   centerx;
int                     shootdelta;           // pixels away from centerx a target can be
fixed                   scale,maxslope;
long                    heightnumerator;
int                     minheightdiv;


void            Quit (char *error);

boolean startgame,loadedgame,virtualreality;
int             mouseadjustment;

char    configname[13]="CONFIG.";


/*
=============================================================================

                                                 LOCAL VARIABLES

=============================================================================
*/


/*
====================
=
= ReadConfig
=
====================
*/

void ReadConfig(void)
{
        int             file;
        SDMode  sd;
        SMMode  sm;
        SDSMode sds;


        if ( (file = open(configname,O_BINARY | O_RDONLY)) != -1)
        {
        //
        // valid config file
        //
                word tmp;
                read(file,&tmp,sizeof(tmp));
                if(tmp!=0xfefc)
                {
                        close(file);
                        goto noconfig;
                }
                read(file,Scores,sizeof(HighScore) * MaxScores);

                read(file,&sd,sizeof(sd));
                read(file,&sm,sizeof(sm));
                read(file,&sds,sizeof(sds));

                read(file,&mouseenabled,sizeof(mouseenabled));
                read(file,&joystickenabled,sizeof(joystickenabled));
                read(file,&joypadenabled,sizeof(joypadenabled));
                read(file,&joystickprogressive,sizeof(joystickprogressive));
                read(file,&joystickport,sizeof(joystickport));

                read(file,dirscan,sizeof(dirscan));
                read(file,buttonscan,sizeof(buttonscan));
                read(file,buttonmouse,sizeof(buttonmouse));
                read(file,buttonjoy,sizeof(buttonjoy));

                read(file,&viewsize,sizeof(viewsize));
                read(file,&mouseadjustment,sizeof(mouseadjustment));

                close(file);

                if ((sd == sdm_AdLib || sm == smm_AdLib) && !AdLibPresent && !SoundBlasterPresent)
                {
                        sd = sdm_PC;
                        sm = smm_Off;
                }

                if ((sds == sds_SoundBlaster && !SoundBlasterPresent) ||
                        (sds == sds_SoundSource && !SoundSourcePresent))
                        sds = sds_Off;

                // make sure values are correct

                if(mouseenabled) mouseenabled=true;
                if(joystickenabled) joystickenabled=true;
                if(joystickprogressive) joystickprogressive = true;
                if(joypadenabled) joypadenabled = true;
                if(joystickport) joystickport = 1;

                if (!MousePresent)
                        mouseenabled = false;
                if (!JoysPresent[joystickport])
                        joystickenabled = false;

                if(mouseadjustment<0) mouseadjustment=0;
                else if(mouseadjustment>9) mouseadjustment=9;

                if(viewsize<4) viewsize=4;
                else if(viewsize>19) viewsize=19;

                MainMenu[6].active=1;
                MainItems.curpos=0;
        }
        else
        {
        //
        // no config file, so select by hardware
        //
noconfig:
                if (SoundBlasterPresent || AdLibPresent)
                {
                        sd = sdm_AdLib;
                        sm = smm_AdLib;
                }
                else
                {
                        sd = sdm_PC;
                        sm = smm_Off;
                }

                if (SoundBlasterPresent)
                        sds = sds_SoundBlaster;
                else if (SoundSourcePresent)
                        sds = sds_SoundSource;
                else
                        sds = sds_Off;

                if (MousePresent)
                        mouseenabled = true;

                joystickenabled = false;
                joypadenabled = false;
                joystickport = 0;
                joystickprogressive = false;

                viewsize = 19;                          // start with a good size
                mouseadjustment=5;
        }

        SD_SetMusicMode (sm);
        SD_SetSoundMode (sd);
        SD_SetDigiDevice (sds);
}


/*
====================
=
= WriteConfig
=
====================
*/

void WriteConfig(void)
{
        int file;

        file = open(configname,O_CREAT | O_BINARY | O_WRONLY,
                                S_IREAD | S_IWRITE | S_IFREG);

        if (file != -1)
        {
                word tmp=0xfefc;
                write(file,&tmp,sizeof(tmp));
                write(file,Scores,sizeof(HighScore) * MaxScores);

                write(file,&SoundMode,sizeof(SoundMode));
                write(file,&MusicMode,sizeof(MusicMode));
                write(file,&DigiMode,sizeof(DigiMode));

                write(file,&mouseenabled,sizeof(mouseenabled));
                write(file,&joystickenabled,sizeof(joystickenabled));
                write(file,&joypadenabled,sizeof(joypadenabled));
                write(file,&joystickprogressive,sizeof(joystickprogressive));
                write(file,&joystickport,sizeof(joystickport));

                write(file,dirscan,sizeof(dirscan));
                write(file,buttonscan,sizeof(buttonscan));
                write(file,buttonmouse,sizeof(buttonmouse));
                write(file,buttonjoy,sizeof(buttonjoy));

                write(file,&viewsize,sizeof(viewsize));
                write(file,&mouseadjustment,sizeof(mouseadjustment));

                close(file);
        }
}


//===========================================================================

/*
=====================
=
= NewGame
=
= Set up new game to start from the beginning
=
=====================
*/

void NewGame (int difficulty,int episode)
{
        memset (&gamestate,0,sizeof(gamestate));
        gamestate.difficulty = difficulty;
        gamestate.weapon = gamestate.bestweapon
                = gamestate.chosenweapon = wp_pistol;
        gamestate.health = 100;
        gamestate.ammo = STARTAMMO;
        gamestate.lives = 3;
        gamestate.nextextra = EXTRAPOINTS;
        gamestate.episode=episode;

        startgame = true;
}

//===========================================================================

void DiskFlopAnim(int x,int y)
{
        static char which=0;
        if (!x && !y)
        return;
        VWB_DrawPic(x,y,C_DISKLOADING1PIC+which);
        VW_UpdateScreen();
        which^=1;
}


long DoChecksum(byte *source,unsigned size,long checksum)
{
        unsigned i;

        for (i=0;i<size-1;i++)
        checksum += source[i]^source[i+1];

        return checksum;
}


/*
==================
=
= SaveTheGame
=
==================
*/

extern statetype s_grdstand;
extern statetype s_player;

boolean SaveTheGame(int file,int x,int y)
{
        struct diskfree_t dfree;
        long avail,size,checksum;
        objtype *ob;
        objtype nullobj;
        statobj_t nullstat;

        if (_dos_getdiskfree(0,&dfree))
          Quit("Error in _dos_getdiskfree call");

        avail = (long)dfree.avail_clusters *
                        dfree.bytes_per_sector *
                        dfree.sectors_per_cluster;

        size = 0;
        for (ob = player; ob ; ob=ob->next)
                size += sizeof(*ob);
        size += sizeof(nullobj);

        size += sizeof(gamestate) +
                        sizeof(LRstruct)*LRpack +
                        sizeof(tilemap) +
                        sizeof(actorat) +
                        sizeof(laststatobj) +
                        sizeof(statobjlist) +
                        sizeof(doorposition) +
                        sizeof(pwallstate) +
                        sizeof(pwalltile) +
                        sizeof(pwallx) +
                        sizeof(pwally) +
                        sizeof(pwalldir) +
                        sizeof(pwallpos);

        if (avail < size)
        {
                Message(STR_NOSPACE1"\n"STR_NOSPACE2);
                return false;
        }

        checksum = 0;

        DiskFlopAnim(x,y);
        write(file,&gamestate,sizeof(gamestate));
        checksum = DoChecksum((byte *)&gamestate,sizeof(gamestate),checksum);

        DiskFlopAnim(x,y);
        write (file,&LevelRatios[0],sizeof(LRstruct)*LRpack);
        checksum = DoChecksum((byte *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

        DiskFlopAnim(x,y);
        write (file,tilemap,sizeof(tilemap));
        checksum = DoChecksum((byte *)tilemap,sizeof(tilemap),checksum);
        DiskFlopAnim(x,y);

        for(int i=0;i<MAPSIZE;i++)
        {
                for(int j=0;j<MAPSIZE;j++)
                {
                        word actnum;
                        objtype *objptr=actorat[i][j];
                        if((long)objptr&0xffff0000)
                                actnum=0x8000 | (word)(objptr-objlist);
                        else
                                actnum=(word)objptr;
                        write(file,&actnum,sizeof(actnum));
                        checksum = DoChecksum((byte *)&actnum,sizeof(actnum),checksum);
                }
        }

        write (file,areaconnect,sizeof(areaconnect));
        write (file,areabyplayer,sizeof(areabyplayer));

        // player object needs special treatment as it's in WL_AGENT.CPP and not in
        // WL_ACT2.CPP which could cause problems for the relative addressing
        
        ob = player;
        DiskFlopAnim(x,y);
        memcpy(&nullobj,ob,sizeof(nullobj));
        nullobj.state=(statetype *) ((long)nullobj.state-(long)&s_player);
        write (file,&nullobj,sizeof(nullobj));
        ob = ob->next;
        
        DiskFlopAnim(x,y);
        for (; ob ; ob=ob->next)
        {
                memcpy(&nullobj,ob,sizeof(nullobj));
                nullobj.state=(statetype *) ((long)nullobj.state-(long)&s_grdstand);
                write (file,&nullobj,sizeof(nullobj));
        }
        nullobj.active = ac_badobject;          // end of file marker
        DiskFlopAnim(x,y);
        write (file,&nullobj,sizeof(nullobj));

        DiskFlopAnim(x,y);
        word laststatobjnum=(word) (laststatobj-statobjlist);
        write (file,&laststatobjnum,sizeof(laststatobjnum));
        checksum = DoChecksum((byte *)&laststatobjnum,sizeof(laststatobjnum),checksum);
        
        DiskFlopAnim(x,y);
        for(int i1=0;i1<MAXSTATS;i1++)
        {
                memcpy(&nullstat,statobjlist+i1,sizeof(nullstat));
                nullstat.visspot=(byte *) ((long) nullstat.visspot-(long)spotvis);
                write (file,&nullstat,sizeof(nullstat));
                checksum = DoChecksum((byte *)&nullstat,sizeof(nullstat),checksum);
        }

        DiskFlopAnim(x,y);
        write (file,doorposition,sizeof(doorposition));
        checksum = DoChecksum((byte *)doorposition,sizeof(doorposition),checksum);
        DiskFlopAnim(x,y);
        write (file,doorobjlist,sizeof(doorobjlist));
        checksum = DoChecksum((byte *)doorobjlist,sizeof(doorobjlist),checksum);

        DiskFlopAnim(x,y);
        write (file,&pwallstate,sizeof(pwallstate));
        checksum = DoChecksum((byte *)&pwallstate,sizeof(pwallstate),checksum);
        write (file,&pwalltile,sizeof(pwalltile));
        checksum = DoChecksum((byte *)&pwalltile,sizeof(pwalltile),checksum);
        write (file,&pwallx,sizeof(pwallx));
        checksum = DoChecksum((byte *)&pwallx,sizeof(pwallx),checksum);
        write (file,&pwally,sizeof(pwally));
        checksum = DoChecksum((byte *)&pwally,sizeof(pwally),checksum);
        write (file,&pwalldir,sizeof(pwalldir));
        checksum = DoChecksum((byte *)&pwalldir,sizeof(pwalldir),checksum);
        write (file,&pwallpos,sizeof(pwallpos));
        checksum = DoChecksum((byte *)&pwallpos,sizeof(pwallpos),checksum);

        //
        // WRITE OUT CHECKSUM
        //
        write (file,&checksum,sizeof(checksum));

        write (file,&lastgamemusicoffset,sizeof(lastgamemusicoffset));

        return(true);
}

//===========================================================================

/*
==================
=
= LoadTheGame
=
==================
*/

boolean LoadTheGame(int file,int x,int y)
{
        long checksum,oldchecksum;
        objtype nullobj;
        statobj_t nullstat;
        
        checksum = 0;

        DiskFlopAnim(x,y);
        read (file,&gamestate,sizeof(gamestate));
        checksum = DoChecksum((byte *)&gamestate,sizeof(gamestate),checksum);

        DiskFlopAnim(x,y);
        read (file,&LevelRatios[0],sizeof(LRstruct)*LRpack);
        checksum = DoChecksum((byte *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

        DiskFlopAnim(x,y);
        SetupGameLevel ();

        DiskFlopAnim(x,y);
        read (file,tilemap,sizeof(tilemap));
        checksum = DoChecksum((byte *)tilemap,sizeof(tilemap),checksum);

        DiskFlopAnim(x,y);

        int actnum=0;
        for(int i=0;i<MAPSIZE;i++)
        {
                for(int j=0;j<MAPSIZE;j++)
                {
                        read (file,&actnum,sizeof(word));
                        checksum = DoChecksum((byte *) &actnum,sizeof(word),checksum);
                        if(actnum&0x8000)
                                actorat[i][j]=objlist+(actnum&0x7fff);
                        else
                                actorat[i][j]=(objtype *) actnum;
                }
        }

        read (file,areaconnect,sizeof(areaconnect));
        read (file,areabyplayer,sizeof(areabyplayer));

        InitActorList ();
        DiskFlopAnim(x,y);
        read (file,player,sizeof(*player));
        player->state=(statetype *) ((long)player->state+(long)&s_player);

        while (1)
        {
                DiskFlopAnim(x,y);
                read (file,&nullobj,sizeof(nullobj));
                if (nullobj.active == ac_badobject)
                        break;
                GetNewActor ();
                nullobj.state=(statetype *) ((long)nullobj.state+(long)&s_grdstand);
         // don't copy over the links
                memcpy (newobj,&nullobj,sizeof(nullobj)-8);
        }

        DiskFlopAnim(x,y);
        word laststatobjnum;
        read (file,&laststatobjnum,sizeof(laststatobjnum));
        laststatobj=statobjlist+laststatobjnum;
        checksum = DoChecksum((byte *)&laststatobjnum,sizeof(laststatobjnum),checksum);

        DiskFlopAnim(x,y);
        for(int i2=0;i2<MAXSTATS;i2++)
        {
                read(file,&nullstat,sizeof(nullstat));
                checksum = DoChecksum((byte *)&nullstat,sizeof(nullstat),checksum);
                nullstat.visspot=(byte *) ((long)nullstat.visspot+(long)spotvis);
                memcpy(statobjlist+i2,&nullstat,sizeof(nullstat));
        }               

        DiskFlopAnim(x,y);
        read (file,doorposition,sizeof(doorposition));
        checksum = DoChecksum((byte *)doorposition,sizeof(doorposition),checksum);
        DiskFlopAnim(x,y);
        read (file,doorobjlist,sizeof(doorobjlist));
        checksum = DoChecksum((byte *)doorobjlist,sizeof(doorobjlist),checksum);

        DiskFlopAnim(x,y);
        read (file,&pwallstate,sizeof(pwallstate));
        checksum = DoChecksum((byte *)&pwallstate,sizeof(pwallstate),checksum);
        read (file,&pwalltile,sizeof(pwalltile));
        checksum = DoChecksum((byte *)&pwalltile,sizeof(pwalltile),checksum);
        read (file,&pwallx,sizeof(pwallx));
        checksum = DoChecksum((byte *)&pwallx,sizeof(pwallx),checksum);
        read (file,&pwally,sizeof(pwally));
        checksum = DoChecksum((byte *)&pwally,sizeof(pwally),checksum);
        read (file,&pwalldir,sizeof(pwalldir));
        checksum = DoChecksum((byte *)&pwalldir,sizeof(pwalldir),checksum);
        read (file,&pwallpos,sizeof(pwallpos));
        checksum = DoChecksum((byte *)&pwallpos,sizeof(pwallpos),checksum);
       
        if (gamestate.secretcount)      // assign valid floorcodes under moved pushwalls
        {
                word *map, *obj; word tile, sprite;
                map = mapsegs[0]; obj = mapsegs[1];
                for (y=0;y<mapheight;y++)
                        for (x=0;x<mapwidth;x++)
                        {
                                tile = *map++; sprite = *obj++;
                                if (sprite == PUSHABLETILE && !tilemap[x][y]
                                    && (tile < AREATILE || tile >= (AREATILE+NUMMAPS)))
                                {
                                        if (*map >= AREATILE)
                                                tile = *map;
                                        if (*(map-1-mapwidth) >= AREATILE)
                                                tile = *(map-1-mapwidth);
                                        if (*(map-1+mapwidth) >= AREATILE)
                                                tile = *(map-1+mapwidth);
                                        if ( *(map-2) >= AREATILE)
                                                tile = *(map-2);

                                        *(map-1) = tile; *(obj-1) = 0;
                                }
                        }
        }

        Thrust(0,0);    // set player->areanumber to the floortile you're standing on

        read (file,&oldchecksum,sizeof(oldchecksum));
        
        read (file,&lastgamemusicoffset,sizeof(lastgamemusicoffset));
        if(lastgamemusicoffset<0) lastgamemusicoffset=0;

        
        if (oldchecksum != checksum)
        {
                Message(STR_SAVECHT1"\n"
                         STR_SAVECHT2"\n"
                         STR_SAVECHT3"\n"
                         STR_SAVECHT4);

                IN_ClearKeysDown();
                IN_Ack();

                gamestate.oldscore = gamestate.score = 0;
                gamestate.lives = 1;
                gamestate.weapon =
           gamestate.chosenweapon =
           gamestate.bestweapon = wp_pistol;
                gamestate.ammo = 8;
        }

        return true;
}

//===========================================================================

/*
==========================
=
= ShutdownId
=
= Shuts down all ID_?? managers
=
==========================
*/

void ShutdownId (void)
{
        US_Shutdown ();         // This line is completely useless...
        SD_Shutdown ();
        PM_Shutdown ();
        IN_Shutdown ();
        VW_Shutdown ();
        CA_Shutdown ();
}


//===========================================================================

/*
==================
=
= BuildTables
=
= Calculates:
=
= scale                 projection constant
= sintable/costable     overlapping fractional tables
=
==================
*/

const   float   radtoint = (float)FINEANGLES/2/PI;

void BuildTables (void)
{
//
// calculate fine tangents
//

    for(int i=0;i<FINEANGLES/8;i++)
    {
        double tang=tan((i+0.5)/radtoint);
        finetangent[i]=(long)(tang*GLOBAL1);
        finetangent[FINEANGLES/4-1-i]=(long)((1/tang)*GLOBAL1);
    }

//
// costable overlays sintable with a quarter phase shift
// ANGLES is assumed to be divisable by four
//

    float angle=0;
    float anglestep=(float)(PI/2/ANGLEQUAD);
    for(int i3=0;i3<=ANGLEQUAD;i3++)
    {
        fixed value=(long)(GLOBAL1*sin(angle));
        sintable[i3]=sintable[i3+ANGLES]=sintable[ANGLES/2-i3]=value;
        sintable[ANGLES-i3]=sintable[ANGLES/2+i3]=-value;
        angle+=anglestep;
    }
}

//===========================================================================


/*
====================
=
= CalcProjection
=
= Uses focallength
=
====================
*/

void CalcProjection (long focal)
{
        int     i;
        int    intang;
        float   angle;
        double  tang;
        int     halfview;
        double  facedist;

        focallength = focal;
        facedist = focal+MINDIST;
        halfview = viewwidth/2;                                 // half view in pixels

//
// calculate scale value for vertical height calculations
// and sprite x calculations
//
        scale = halfview*facedist/(VIEWGLOBAL/2);

//
// divide heightnumerator by a posts distance to get the posts height for
// the heightbuffer.  The pixel height is height>>2
//
        heightnumerator = (TILEGLOBAL*scale)>>6;
        minheightdiv = heightnumerator/0x7fff +1;

//
// calculate the angle offset from view angle of each pixel's ray
//

        for (i=0;i<halfview;i++)
        {
        // start 1/2 pixel over, so viewangle bisects two middle pixels
                tang = (long)i*VIEWGLOBAL/viewwidth/facedist;
                angle = atan(tang);
                intang = angle*radtoint;
                pixelangle[halfview-1-i] = intang;
                pixelangle[halfview+i] = -intang;
        }

//
// if a point's abs(y/x) is greater than maxslope, the point is outside
// the view area
//
        maxslope = finetangent[pixelangle[0]];
        maxslope >>= 8;
}



//===========================================================================

/*
===================
=
= SetupWalls
=
= Map tile values to scaled pics
=
===================
*/

void SetupWalls (void)
{
        int     i;

   horizwall[0]=0;
   vertwall[0]=0;

        for (i=1;i<MAXWALLTILES;i++)
        {
                horizwall[i]=(i-1)*2;
                vertwall[i]=(i-1)*2+1;
        }
}

//===========================================================================

/*
==========================
=
= SignonScreen
=
==========================
*/

void SignonScreen (void)                        // VGA version
{
//      unsigned        segstart,seglength;

        VL_SetVGAPlaneMode ();
        VL_SetPalette (gamepal);

        if (!virtualreality)
        {
                VW_SetScreen(0x8000,0);
                VL_MungePic (signon,320,200);
                VL_MemToScreen (signon,320,200,0,0);
                VW_SetScreen(0,0);
        }

// TODO: The signon memory does NOT become recycled anymore!!!

#ifdef ABCAUS
//
// reclaim the memory from the linked in signon screen
//
        segstart = FP_SEG(&introscn);
        seglength = 64000/16;
        if (FP_OFF(&introscn))
        {
                segstart++;
                seglength--;
        }
        MML_UseSpace (segstart,seglength);
#endif
}


/*
==========================
=
= FinishSignon
=
==========================
*/

void FinishSignon (void)
{
#ifndef SPEAR
        VW_Bar (0,189,300,11,*(byte *)0xa0000);
        WindowX = 0;
        WindowW = 320;
        PrintY = 190;

        #ifndef JAPAN
        SETFONTCOLOR(14,4);

        #ifdef SPANISH
        US_CPrint ("Oprima una tecla");
        #else
        US_CPrint ("Press a key");
        #endif

        #endif

        if (!NoWait)
                IN_Ack ();

        #ifndef JAPAN
        VW_Bar (0,189,300,11,*(byte *)0xa0000);

        PrintY = 190;
        SETFONTCOLOR(10,4);

        #ifdef SPANISH
        US_CPrint ("pensando...");
        #else
        US_CPrint ("Working...");
        #endif

        #endif

        SETFONTCOLOR(0,15);
#else
        if (!NoWait)
                VW_WaitVBL(3*70);
#endif
}

//===========================================================================

/*
=================
=
= MS_CheckParm
=
=================
*/

boolean MS_CheckParm (char *check)
{
        int             i;
        char    *parm;

        for (i = 1;i<__argc;i++)
        {
                parm = __argv[i];

                while ( !isalpha(*parm) )       // skip - / \ etc.. in front of parm
                        if (!*parm++)
                                break;                          // hit end of string without an alphanum

                if ( !stricmp(check,parm) )
                        return true;
        }

        return false;
}

//===========================================================================

/*
=====================
=
= InitDigiMap
=
=====================
*/

static  int     wolfdigimap[] =
                {
                        // These first sounds are in the upload version
#ifndef SPEAR
                        HALTSND,                0,
                        DOGBARKSND,             1,
                        CLOSEDOORSND,           2,
                        OPENDOORSND,            3,
                        ATKMACHINEGUNSND,       4,
                        ATKPISTOLSND,           5,
                        ATKGATLINGSND,          6,
                        SCHUTZADSND,            7,
                        GUTENTAGSND,            8,
                        MUTTISND,               9,
                        BOSSFIRESND,            10,
                        SSFIRESND,              11,
                        DEATHSCREAM1SND,        12,
                        DEATHSCREAM2SND,        13,
                        DEATHSCREAM3SND,        13,
                        TAKEDAMAGESND,          14,
                        PUSHWALLSND,            15,

                        LEBENSND,               20,
                        NAZIFIRESND,            21,
                        SLURPIESND,             22,

                        YEAHSND,                                32,

#ifndef UPLOAD
                        // These are in all other episodes
                        DOGDEATHSND,            16,
                        AHHHGSND,               17,
                        DIESND,                 18,
                        EVASND,                 19,

                        TOT_HUNDSND,            23,
                        MEINGOTTSND,            24,
                        SCHABBSHASND,           25,
                        HITLERHASND,            26,
                        SPIONSND,               27,
                        NEINSOVASSND,           28,
                        DOGATTACKSND,           29,
                        LEVELDONESND,           30,
                        MECHSTEPSND,                    31,

                        SCHEISTSND,                             33,
                        DEATHSCREAM4SND,                34,             // AIIEEE
                        DEATHSCREAM5SND,                35,             // DEE-DEE
                        DONNERSND,                              36,             // EPISODE 4 BOSS DIE
                        EINESND,                                37,             // EPISODE 4 BOSS SIGHTING
                        ERLAUBENSND,                    38,             // EPISODE 6 BOSS SIGHTING
                        DEATHSCREAM6SND,                39,             // FART
                        DEATHSCREAM7SND,                40,             // GASP
                        DEATHSCREAM8SND,                41,             // GUH-BOY!
                        DEATHSCREAM9SND,                42,             // AH GEEZ!
                        KEINSND,                                43,             // EPISODE 5 BOSS SIGHTING
                        MEINSND,                                44,             // EPISODE 6 BOSS DIE
                        ROSESND,                                45,             // EPISODE 5 BOSS DIE

#endif
#else
//
// SPEAR OF DESTINY DIGISOUNDS
//
                        HALTSND,                0,
                        CLOSEDOORSND,           2,
                        OPENDOORSND,            3,
                        ATKMACHINEGUNSND,       4,
                        ATKPISTOLSND,           5,
                        ATKGATLINGSND,          6,
                        SCHUTZADSND,            7,
                        BOSSFIRESND,            8,
                        SSFIRESND,              9,
                        DEATHSCREAM1SND,        10,
                        DEATHSCREAM2SND,        11,
                        TAKEDAMAGESND,          12,
                        PUSHWALLSND,            13,
                        AHHHGSND,               15,
                        LEBENSND,               16,
                        NAZIFIRESND,            17,
                        SLURPIESND,             18,
                        LEVELDONESND,           22,
                        DEATHSCREAM4SND,                23,             // AIIEEE
                        DEATHSCREAM3SND,        23,             // DOUBLY-MAPPED!!!
                        DEATHSCREAM5SND,                24,             // DEE-DEE
                        DEATHSCREAM6SND,                25,             // FART
                        DEATHSCREAM7SND,                26,             // GASP
                        DEATHSCREAM8SND,                27,             // GUH-BOY!
                        DEATHSCREAM9SND,                28,             // AH GEEZ!
                        GETGATLINGSND,                  38,             // Got Gat replacement

#ifndef SPEARDEMO
                        DOGBARKSND,             1,
                        DOGDEATHSND,            14,
                        SPIONSND,               19,
                        NEINSOVASSND,           20,
                        DOGATTACKSND,           21,
                        TRANSSIGHTSND,                  29,             // Trans Sight
                        TRANSDEATHSND,                  30,             // Trans Death
                        WILHELMSIGHTSND,                31,             // Wilhelm Sight
                        WILHELMDEATHSND,                32,             // Wilhelm Death
                        UBERDEATHSND,                   33,             // Uber Death
                        KNIGHTSIGHTSND,                 34,             // Death Knight Sight
                        KNIGHTDEATHSND,                 35,             // Death Knight Death
                        ANGELSIGHTSND,                  36,             // Angel Sight
                        ANGELDEATHSND,                  37,             // Angel Death
                        GETSPEARSND,                    39,             // Got Spear replacement
#endif
#endif
                        LASTSOUND
                };


void InitDigiMap (void)
{
        int *map;

        for (map = wolfdigimap;*map != LASTSOUND;map += 2)
                DigiMap[map[0]] = map[1];
}

#ifndef SPEAR
CP_iteminfo     MusicItems={CTL_X,CTL_Y,6,0,32};
CP_itemtype MusicMenu[]=
        {
                {1,"Get Them!",0},
                {1,"Searching",0},
                {1,"P.O.W.",0},
                {1,"Suspense",0},
                {1,"War March",0},
                {1,"Around The Corner!",0},

                {1,"Nazi Anthem",0},
                {1,"Lurking...",0},
                {1,"Going After Hitler",0},
                {1,"Pounding Headache",0},
                {1,"Into the Dungeons",0},
                {1,"Ultimate Conquest",0},

                {1,"Kill the S.O.B.",0},
                {1,"The Nazi Rap",0},
                {1,"Twelfth Hour",0},
                {1,"Zero Hour",0},
                {1,"Ultimate Conquest",0},
                {1,"Wolfpack",0}
        };
#else
CP_iteminfo MusicItems={CTL_X,CTL_Y-20,9,0,32};
CP_itemtype MusicMenu[]=
   {
                {1,"Funky Colonel Bill",0},
                {1,"Death To The Nazis",0},
                {1,"Tiptoeing Around",0},
                {1,"Is This THE END?",0},
                {1,"Evil Incarnate",0},
                {1,"Jazzin' Them Nazis",0},
                {1,"Puttin' It To The Enemy",0},
                {1,"The SS Gonna Get You",0},
                {1,"Towering Above",0}
        };
#endif

#ifndef SPEARDEMO
void DoJukebox(void)
{
        int which,lastsong=-1;
        unsigned start;
        unsigned songs[]=
                {
#ifndef SPEAR
                        GETTHEM_MUS,
                        SEARCHN_MUS,
                        POW_MUS,
                        SUSPENSE_MUS,
                        WARMARCH_MUS,
                        CORNER_MUS,

                        NAZI_OMI_MUS,
                        PREGNANT_MUS,
                        GOINGAFT_MUS,
                        HEADACHE_MUS,
                        DUNGEON_MUS,
                        ULTIMATE_MUS,

                        INTROCW3_MUS,
                        NAZI_RAP_MUS,
                        TWELFTH_MUS,
                        ZEROHOUR_MUS,
                        ULTIMATE_MUS,
                        PACMAN_MUS
#else
                        XFUNKIE_MUS,             // 0
                        XDEATH_MUS,              // 2
                        XTIPTOE_MUS,             // 4
                        XTHEEND_MUS,             // 7
                        XEVIL_MUS,               // 17
                        XJAZNAZI_MUS,            // 18
                        XPUTIT_MUS,              // 21
                        XGETYOU_MUS,             // 22
                        XTOWER2_MUS              // 23
#endif
                };
#ifndef SPEAR
#ifndef UPLOAD
        struct dostime_t time;
#endif
#endif

        IN_ClearKeysDown();
        if (!AdLibPresent && !SoundBlasterPresent)
                return;

        MenuFadeOut();

#ifndef SPEAR
#ifndef UPLOAD
        _dos_gettime(&time);
        start = (time.hsecond%3)*6;
#else
        start = 0;
#endif
#else
        start = 0;
#endif

        CA_CacheGrChunk (STARTFONT+1);
#ifdef SPEAR
        CacheLump (BACKDROP_LUMP_START,BACKDROP_LUMP_END);
#else
        CacheLump (CONTROLS_LUMP_START,CONTROLS_LUMP_END);
#endif
        CA_LoadAllSounds ();

        fontnumber=1;
        ClearMScreen ();
        VWB_DrawPic(112,184,C_MOUSELBACKPIC);
        DrawStripes (10);
        SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);

#ifndef SPEAR
        DrawWindow (CTL_X-2,CTL_Y-6,280,13*7,BKGDCOLOR);
#else
        DrawWindow (CTL_X-2,CTL_Y-26,280,13*10,BKGDCOLOR);
#endif

        DrawMenu (&MusicItems,&MusicMenu[start]);

        SETFONTCOLOR (READHCOLOR,BKGDCOLOR);
        PrintY=15;
        WindowX = 0;
        WindowY = 320;
        US_CPrint ("Robert's Jukebox");

        SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);
        VW_UpdateScreen();
        MenuFadeIn();

        do
        {
                which = HandleMenu(&MusicItems,&MusicMenu[start],NULL);
                if (which>=0)
                {
                        if (lastsong >= 0)
                                MusicMenu[start+lastsong].active = 1;

                        StartCPMusic(songs[start + which]);
                        MusicMenu[start+which].active = 2;
                        DrawMenu (&MusicItems,&MusicMenu[start]);
                        VW_UpdateScreen();
                        lastsong = which;
                }
        } while(which>=0);

        MenuFadeOut();
        IN_ClearKeysDown();
#ifdef SPEAR
        UnCacheLump (BACKDROP_LUMP_START,BACKDROP_LUMP_END);
#else
        UnCacheLump (CONTROLS_LUMP_START,CONTROLS_LUMP_END);
#endif
}
#endif

/*
==========================
=
= InitGame
=
= Load a few things right away
=
==========================
*/

void InitGame (void)
{
#ifndef SPEARDEMO
        boolean didjukebox=false;
#endif
        SignonScreen ();

        IN_Startup ();
        PM_Startup ();
        SD_Startup ();
        CA_Startup ();
        US_Startup ();

        // TODO: Will any memory checking be needed someday??
#ifdef ABCAUS
#ifndef SPEAR
        if (mminfo.mainmem < 235000L)
#else
        if (mminfo.mainmem < 257000L && !MS_CheckParm("debugmode"))
#endif
        {
                byte *screen;

                CA_CacheGrChunk (ERRORSCREEN);
                screen = grsegs[ERRORSCREEN];
                ShutdownId();
                memcpy((byte *)0xb8000,screen+7+7*160,17*160);
                gotoxy (1,23);
                exit(1);
        }
#endif


//
// build some tables
//
        InitDigiMap ();

        ReadConfig ();


//
// HOLDING DOWN 'M' KEY?
//
#ifndef SPEARDEMO
        if (Keyboard[sc_M])
        {
                DoJukebox();
                didjukebox=true;
        }
        else
#endif

//
// draw intro screen stuff
//
        IntroScreen ();

//
// load in and lock down some basic chunks
//

        CA_CacheGrChunk(STARTFONT);

        LoadLatchMem ();
        BuildTables ();          // trig tables
        SetupWalls ();

        NewViewSize (viewsize);

//
// initialize variables
//
        InitRedShifts ();
#ifndef SPEARDEMO
        if(!didjukebox)
#endif
                FinishSignon();

        vdisp = (byte *) (0xa0000+PAGE1START);
        vbuf = (byte *) (0xa0000+PAGE2START);
}

//===========================================================================

/*
==========================
=
= SetViewSize
=
==========================
*/

boolean SetViewSize (unsigned width, unsigned height)
{
        viewwidth = width&~15;                  // must be divisable by 16
        viewheight = height&~1;                 // must be even
        centerx = viewwidth/2-1;
        shootdelta = viewwidth/10;
        screenofs = ((200-STATUSLINES-viewheight)/2*SCREENWIDTH+(320-viewwidth)/8);

//
// calculate trace angles and projection constants
//
        CalcProjection (FOCALLENGTH);

        return true;
}


void ShowViewSize (int width)
{
        int     oldwidth,oldheight;

        oldwidth = viewwidth;
        oldheight = viewheight;

        viewwidth = width*16;
        viewheight = width*16*HEIGHTRATIO;
        DrawPlayBorder ();

        viewheight = oldheight;
        viewwidth = oldwidth;
}


void NewViewSize (int width)
{
        viewsize = width;
        SetViewSize (width*16,width*16*HEIGHTRATIO);
}



//===========================================================================

/*
==========================
=
= Quit
=
==========================
*/

void Quit (char *error)
{
        byte *screen;

        if (!pictable)  // don't try to display the red box before it's loaded
        {
                ShutdownId();
                if (error && *error)
                {
                        SetTextCursor(0,0);
                        puts(error);
                        SetTextCursor(0,2);
                        VW_WaitVBL(100);
                }
                exit(1);
        }

        if (!error || !*error)
        {
         #ifndef JAPAN
         CA_CacheGrChunk (ORDERSCREEN);
         screen = grsegs[ORDERSCREEN];
         #endif
         WriteConfig ();
        }
        else
        {
         CA_CacheGrChunk (ERRORSCREEN);
         screen = grsegs[ERRORSCREEN];
        }

        ShutdownId ();

        if (error && *error)
        {
          memcpy((byte *)0xb8000,screen+7,7*160);
          SetTextCursor(0,0);
          puts(" ");      // temp fix for screen pointer assignment
          SetTextCursor(9,3);
          puts(error);          // printf(error);
          SetTextCursor(0,7);
          VW_WaitVBL(200);
          exit(1);
        }
        else
        if (!error || !(*error))
        {
                #ifndef JAPAN
                memcpy((byte *)0xb8000,screen+7,24*160); // 24 for SPEAR/UPLOAD compatibility
                #endif
                SetTextCursor(0,0);
                puts(" ");      // temp fix for screen pointer assignment
                SetTextCursor(0,23);
        }

        exit(0);
}

//===========================================================================



/*
=====================
=
= DemoLoop
=
=====================
*/

static  char *ParmStrings[] = {"baby","easy","normal","hard",""};

void    DemoLoop (void)
{
        static int LastDemo;
        int     i,level;

//
// check for launch from ted
//
        if (tedlevel)
        {
                NoWait = true;
                NewGame(1,0);

                for (i = 1;i < __argc;i++)
                {
                        if ( (level = US_CheckParm(__argv[i],ParmStrings)) != -1)
                        {
                         gamestate.difficulty=level;
                         break;
                        }
                }

#ifndef SPEAR
                gamestate.episode = tedlevelnum/10;
                gamestate.mapon = tedlevelnum%10;
#else
                gamestate.episode = 0;
                gamestate.mapon = tedlevelnum;
#endif
                GameLoop();
                Quit (NULL);
        }


//
// main game cycle
//

#ifndef DEMOTEST

        #ifndef UPLOAD

                #ifndef GOODTIMES
                #ifndef SPEAR
                #ifndef JAPAN
                if (!NoWait)
                        NonShareware();
                #endif
                #else
                        #ifndef GOODTIMES
                        #ifndef SPEARDEMO
                        extern void CopyProtection(void);
                        CopyProtection();
                        #endif
                        #endif
                #endif
                #endif
        #endif

        StartCPMusic(INTROSONG);

#ifndef JAPAN
        if (!NoWait)
                PG13 ();
#endif

#endif

        while (1)
        {
                while (!NoWait)
                {
//
// title page
//
//                      MM_SortMem ();
#ifndef DEMOTEST

#ifdef SPEAR
                        CA_CacheGrChunk (TITLEPALETTE);

                        CA_CacheGrChunk (TITLE1PIC);
                        VWB_DrawPic (0,0,TITLE1PIC);
                        UNCACHEGRCHUNK (TITLE1PIC);

                        CA_CacheGrChunk (TITLE2PIC);
                        VWB_DrawPic (0,80,TITLE2PIC);
                        UNCACHEGRCHUNK (TITLE2PIC);
                        VW_UpdateScreen ();
                        VL_FadeIn(0,255,grsegs[TITLEPALETTE],30);

                        UNCACHEGRCHUNK (TITLEPALETTE);
#else
                        CA_CacheScreen (TITLEPIC);
                        VW_UpdateScreen ();
                        VW_FadeIn();
#endif
                        if (IN_UserInput(TickBase*15))
                                break;
                        VW_FadeOut();
//
// credits page
//
                        CA_CacheScreen (CREDITSPIC);
                        VW_UpdateScreen();
                        VW_FadeIn ();
                        if (IN_UserInput(TickBase*10))
                                break;
                        VW_FadeOut ();
//
// high scores
//
                        DrawHighScores ();
                        VW_UpdateScreen ();
                        VW_FadeIn ();

                        if (IN_UserInput(TickBase*10))
                                break;
#endif
//
// demo
//

                        #ifndef SPEARDEMO
                        PlayDemo (LastDemo++%4);
                        #else
                        PlayDemo (0);
                        #endif

                        if (playstate == ex_abort)
                                break;
                        StartCPMusic(INTROSONG);
                }

                VW_FadeOut ();

#ifdef DEBUGKEYS
#ifndef SPEAR
                if (Keyboard[sc_Tab] && MS_CheckParm("goobers"))
#else
                if (Keyboard[sc_Tab] && MS_CheckParm("debugmode"))
#endif
                        RecordDemo ();
                else
                        US_ControlPanel (0);
#else
                US_ControlPanel (0);
#endif

                if (startgame || loadedgame)
                {
                        GameLoop ();
                        VW_FadeOut();
                        StartCPMusic(INTROSONG);
                }
        }
}


//===========================================================================


/*
==========================
=
= main
=
==========================
*/

void main ()
{
        _fmode=O_BINARY;                // DON'T create save games in text mode!!

#ifdef BETA
        //
        // THIS IS FOR BETA ONLY!
        //
        struct dosdate_t d;

        _dos_getdate(&d);
        if (d.year > YEAR ||
                (d.month >= MONTH && d.day >= DAY))
        {
         printf("Sorry, BETA-TESTING is over. Thanks for you help.\n");
         exit(1);
        }
#endif

        CheckForEpisodes();

        InitGame ();

        DemoLoop();

        Quit("Demo loop exited???");
}

