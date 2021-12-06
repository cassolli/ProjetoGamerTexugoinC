#include <stdlib.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/Audio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define PERSONAGENS_ALTURA 1098
#define PERSONAGENS_LARGURA 860
#define PERSONAGENS_TOTAL 30
#define PERSONAGENS_LINHA 5
#define RESOLUCAOALT 1024
#define RESOLUCAOLAG 768
#define RESOLUCAOMENU 50

typedef struct personagem_t
{
    float posX;
    float posY;
    float tamX;
    float tamY;
    int idxQuadroTexugo;
    int idxQuadroTexugoMin; // quando ativa pode muda o min e max
    int idxQuadroTexugoMax;
    int idxQuadroTexugoMinOriginal;
    int idxQuadroTexugoMaxOriginal;
    float vel;
} personagem;

typedef struct memoria_t
{
    float vida;
    int rifeiroSome;
    int rifeiroTempo;
    int dano;
    int esteroides;
    int chocolate;
    int tiara;
    int detector;
    int pontuacao;
    float defesa;
    int conhecimento;
    int chocolateAtivo;
    int esteroidesAtivo;
    int tiaraAtivo;
    int fase;
    float maisVelTexugo;
    float maisVelInimigo;
    int quantidadecachorro;
    int maiscachorro;
    int quantidaderifeiro;
    int maisrifeiro;
    int quantidadepanda;
    int quantidadedetector;
    int quantidadechocolate;
    int quantidadetiara;
    int quantidadeesteiroides;
    int quantidadeconhecimento;
    int bloqueioBotao;
    int bloqueioVida;
    int tempochocolate;
    int tempoesteroides;
    int tempotiara;
    int tela;
    int posicao;
    int jogando;
} memoria;

typedef struct inimigo_t
{
    float vida;
    float posX;
    float posY;
    float tamX;
    float tamY;
    int idxQuadroInimigo;
    int idxQuadroInimigoMin;
    int idxQuadroInimigoMax;
    int sobe;
    int some;
    int tempo;
    float vel;
} inimigo;

typedef struct poderes_t
{
    int poder;   // 1- Chocolate | 2 - Tiara | 3 - Esteroides | 4 Detector de Rifeiros
    float posX;
    float posY;
    float tamX;
    float tamY;
    int idxQuadro;
    int idxQuadroTexugoMin;
    int idxQuadroTexugoMax;
    int idxQuadroTexugoMin2;
    int idxQuadroTexugoMax2;
    int idxQuadroMin;
    int idxQuadroMax;
    int tempo;
    int pontuacao;
    int some;
} poderes;

sfIntRect PosicaoQuadro(int idxQuadro)
{
    int Linha  = idxQuadro / PERSONAGENS_LINHA;
    int Coluna = idxQuadro % PERSONAGENS_LINHA;

    sfIntRect retorno;
    retorno.height = PERSONAGENS_ALTURA / (PERSONAGENS_TOTAL / PERSONAGENS_LINHA );
    retorno.width  = PERSONAGENS_LARGURA / PERSONAGENS_LINHA;
    retorno.left   = retorno.width  * Coluna;
    retorno.top    = retorno.height * Linha;

    return retorno;
}

int colisao(personagem_t *texugo, inimigo_t *inimigo, memoria_t *dados)
{
    if (texugo->posY+texugo->tamY < inimigo->posY) return false;
    else if(texugo->posY > inimigo->posY+inimigo->tamY) return false;
    else if(texugo->posX+texugo->tamX < inimigo->posX) return false;
    else if(texugo->posX > inimigo->posX+inimigo->tamX) return false;
    dados->vida += inimigo->vida + dados->defesa;
    dados->dano = 1;
}

int colisaoParede(personagem_t *texugo, inimigo_t *inimigo)
{
    if (texugo->posY+(texugo->tamY+20) < inimigo->posY) return false;
    else if(texugo->posY > inimigo->posY+(inimigo->tamY+20)) return false;
    else if(texugo->posX+(texugo->tamX+20) < inimigo->posX) return false;
    else if(texugo->posX > inimigo->posX+(inimigo->tamX+20)) return false;
    else return true;
}

int colisaoPoder(personagem_t *texugo, poderes_t *poder)
{
    if (texugo->posY+texugo->tamY < poder->posY) return false;
    else if(texugo->posY > poder->posY+poder->tamY) return false;
    else if(texugo->posX+texugo->tamX < poder->posX) return false;
    else if(texugo->posX > poder->posX+poder->tamX) return false;
    return true;
}

void caminhaTexugo(personagem_t *texugo, sfSprite *desenho,sfRenderWindow *janela, memoria_t *dados)
{
    if(dados->posicao == 0) texugo->idxQuadroTexugo = texugo->idxQuadroTexugoMin;
    if(dados->posicao == 1) texugo->idxQuadroTexugo = texugo->idxQuadroTexugoMax;

    //printf("quadro %d\n",texugo->idxQuadroTexugo);
    sfSprite_setPosition(desenho, (sfVector2f){texugo->posX, texugo->posY});
    sfIntRect quadro = PosicaoQuadro(texugo->idxQuadroTexugo);
    sfSprite_setTextureRect(desenho, quadro);
    sfRenderWindow_drawSprite( janela, desenho,NULL);
}

void caminhaRifeiro(inimigo_t *rifeiro, personagem_t *texugo, memoria_t *dados, sfSprite *desenho,sfRenderWindow *janela, poderes_t *poder)
{

    if(dados->rifeiroSome == 0)
    {
        if(rifeiro->posX > texugo->posX) rifeiro->posX -= rifeiro->vel+dados->maisVelInimigo;
        if(rifeiro->posX < texugo->posX) rifeiro->posX += rifeiro->vel+dados->maisVelInimigo;
        if(rifeiro->posY > texugo->posY) rifeiro->posY -= rifeiro->vel+dados->maisVelInimigo;
        if(rifeiro->posY < texugo->posY) rifeiro->posY += rifeiro->vel+dados->maisVelInimigo;
        if(dados->dano == 0)
        {
            colisao(texugo,rifeiro, dados);
        }
        sfSprite_setPosition(desenho, (sfVector2f){rifeiro->posX, rifeiro->posY});
        sfIntRect quadro = PosicaoQuadro(rifeiro->idxQuadroInimigo);
        sfSprite_setTextureRect(desenho, quadro);
        sfRenderWindow_drawSprite( janela, desenho,NULL);
    }

    if( dados->rifeiroSome == 1)
    {
        //printf("Rifeiro %d :: %d",dados->rifeiroTempo,poder->tempo);
        if (++dados->rifeiroTempo == poder->tempo)
        {
            dados->rifeiroSome = 0;
            dados->rifeiroTempo = 0;
            texugo->idxQuadroTexugoMin = 0;
            texugo->idxQuadroTexugoMax = 1;
            texugo->idxQuadroTexugo =  texugo->idxQuadroTexugoMin;
        }
    }

}

void caminhaCachorro(inimigo_t *cachorro, personagem_t *texugo, memoria_t *dados,sfSprite *desenho,sfRenderWindow *janela)
{
    cachorro->idxQuadroInimigo++;
    if(cachorro->idxQuadroInimigo>cachorro->idxQuadroInimigoMax) cachorro->idxQuadroInimigo=cachorro->idxQuadroInimigoMin;

    if(dados->tiaraAtivo && colisaoParede(texugo, cachorro))
    {
        if(cachorro->sobe == 1) cachorro->sobe = 0;
        if(cachorro->sobe == 0) cachorro->sobe = 1;
    }

    if(cachorro->sobe == 1) cachorro->posY += cachorro->vel+dados->maisVelInimigo;
    if(cachorro->sobe == 0) cachorro->posY -= cachorro->vel+dados->maisVelInimigo;
    if(cachorro->posY  >= (RESOLUCAOLAG - 55.0f)) cachorro->sobe=0;
    if(cachorro->posY  <= 0.00f) cachorro->sobe=1;

    if(dados->dano == 0)
    {
        colisao(texugo,cachorro, dados);
    }
    sfSprite_setPosition(desenho, (sfVector2f)
    {
        cachorro->posX, cachorro->posY
    });
    sfIntRect quadro = PosicaoQuadro(cachorro->idxQuadroInimigo);
    sfSprite_setTextureRect(desenho, quadro);
    sfRenderWindow_drawSprite( janela, desenho,NULL);
}

void caminhaPanda(inimigo_t *panda, personagem_t *texugo, memoria_t *dados, sfSprite *desenho,sfRenderWindow *janela)
{
    panda->idxQuadroInimigo++;
    if(panda->idxQuadroInimigo > panda->idxQuadroInimigoMax) panda->idxQuadroInimigo = panda->idxQuadroInimigoMin;
    if(dados->tiaraAtivo &&  colisaoParede(texugo, panda))
    {
        if(panda->sobe == 1) panda->sobe = 0;
        if(panda->sobe == 0) panda->sobe = 1;
    }
    if(panda->sobe == 1)
    {
        panda->posX += panda->vel;
        panda->posY -= panda->vel;
        if(panda->posY <= 0.0f | panda->posX >= (RESOLUCAOALT-40.0f)) panda->sobe = 0;
    }

    if(panda->sobe == 0)
    {
        panda->posX -= panda->vel;
        panda->posY += panda->vel;
        if(panda->posY >= (RESOLUCAOLAG-35.0f) | panda->posX <= 0.0f) panda->sobe = 1;
    }
    //printf("%d:X:%.2f:Y:%.2f\n",panda->sobe,panda->posX,panda->posY);
    if(dados->dano == 0)
    {
        colisao(texugo,panda,dados);
    }

    sfSprite_setPosition(desenho, (sfVector2f)
    {
        panda->posX, panda->posY
    });
    sfIntRect quadro = PosicaoQuadro(panda->idxQuadroInimigo);
    sfSprite_setTextureRect(desenho, quadro);
    sfRenderWindow_drawSprite( janela, desenho,NULL);
}

void inserePoder(poderes_t *poder, sfSprite *desenho,sfRenderWindow *janela)
{
    sfSprite_setPosition(desenho, (sfVector2f)
    {
        poder->posX, poder->posY
    });
    sfIntRect quadro = PosicaoQuadro(poder->idxQuadro);
    sfSprite_setTextureRect(desenho, quadro);
    sfRenderWindow_drawSprite( janela, desenho,NULL);
}

void ativaDetector(poderes_t *poder, memoria_t *dados, personagem_t *texugo)
{
    if(dados->detector)
    {
        dados->rifeiroSome = 1;
        if(dados->bloqueioBotao == 0 )
        {
            dados->detector--;
            dados->bloqueioBotao = 1;
            texugo->idxQuadroTexugo     =  poder->idxQuadroTexugoMin;
            texugo->idxQuadroTexugoMin  =  poder->idxQuadroTexugoMin;
            texugo->idxQuadroTexugoMax  =  poder->idxQuadroTexugoMax;
        }
    }
    else
    {
        printf("Sem detectores");
    }
}

void ativaChocolate(poderes_t *poder, memoria_t *dados, personagem_t *texugo){
    if(dados->chocolate)
    {
        //printf("chocolate %d::%d::%d\n",chocolate[0]->idxQuadroTexugoMin,chocolate[0]->idxQuadroTexugoMin,chocolate[0]->idxQuadroTexugoMax);
        if(dados->bloqueioBotao == 0 )
        {
            dados->chocolate--;
            dados->bloqueioBotao = 1;
            dados->chocolateAtivo = 1;
            texugo->idxQuadroTexugo     =  poder->idxQuadroTexugoMin;
            texugo->idxQuadroTexugoMin  =  poder->idxQuadroTexugoMin;
            texugo->idxQuadroTexugoMax  =  poder->idxQuadroTexugoMax;
            texugo->vel += 0.3f;
        }
    }
    else
    {
        printf("Sem chocolate");
    }
}

void inativaChocolate(poderes_t *poder, memoria_t *dados, personagem_t *texugo){
                if(++dados->tempochocolate == poder->tempo)
                {
                    texugo->idxQuadroTexugo  = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMin = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMax = texugo->idxQuadroTexugoMaxOriginal;
                    texugo->vel -= 0.3f;
                    dados->chocolateAtivo =0;
                    dados->tempochocolate =0;
                }
}

void ativaEsteroides(poderes_t *poder, memoria_t *dados, personagem_t *texugo){
    if(dados->esteroides)
                {
                    if(dados->bloqueioBotao == 0 )
                    {
                        dados->esteroides--;
                        dados->bloqueioBotao = 1;
                        dados->esteroidesAtivo = 1;
                        texugo->idxQuadroTexugo     =  poder->idxQuadroTexugoMin;
                        texugo->idxQuadroTexugoMin  =  poder->idxQuadroTexugoMin;
                        texugo->idxQuadroTexugoMax  =  poder->idxQuadroTexugoMax;
                        dados->defesa = 0.6f;
                    }
                }
                else
                {
                    printf("Sem esteroides");
                }
}

void inativaEsteroides(poderes_t *poder, memoria_t *dados, personagem_t *texugo){
            if(dados->esteroidesAtivo ==1 )
            {
                if(++dados->tempoesteroides == poder->tempo)
                {
                    texugo->idxQuadroTexugo = poder->idxQuadroTexugoMin2;
                    texugo->idxQuadroTexugoMin = poder->idxQuadroTexugoMin2;
                    texugo->idxQuadroTexugoMax = poder->idxQuadroTexugoMax2;
                    dados->esteroidesAtivo = 3;
                    dados->tempoesteroides = 0;
                    dados->defesa = -0.6f;

                }
            }
            if(dados->esteroidesAtivo ==3)
            {
                if(++dados->tempoesteroides == (poder->tempo/2))
                {
                    texugo->idxQuadroTexugo = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMin = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMax = texugo->idxQuadroTexugoMaxOriginal;
                    dados->esteroidesAtivo =0;
                    dados->defesa = 0.0f;
                    dados->tempoesteroides = 0;
                    dados->vida = round(dados->vida);
                }
            }
}

void ativaTiara(poderes_t *poder, memoria_t *dados, personagem_t *texugo){

if(dados->tiara)
                {
                    if(dados->bloqueioBotao == 0 )
                    {
                        dados->tiara--;
                        dados->bloqueioBotao = 1;
                        dados->tiaraAtivo = 1;
                        texugo->idxQuadroTexugo     =  poder->idxQuadroTexugoMin;
                        texugo->idxQuadroTexugoMin  =  poder->idxQuadroTexugoMin;
                        texugo->idxQuadroTexugoMax  =  poder->idxQuadroTexugoMax;
                    }
                }
                else
                {
                    printf("Sem tiara");
                }
}

void inativaTiara(poderes_t *poder, memoria_t *dados, personagem_t *texugo){

                if(++dados->tempotiara == poder->tempo)
                {
                    texugo->idxQuadroTexugo = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMin = texugo->idxQuadroTexugoMinOriginal;
                    texugo->idxQuadroTexugoMax = texugo->idxQuadroTexugoMaxOriginal;
                    dados->tempotiara = 0;
                    dados->tiaraAtivo  = 0;

                }
}

void musica(memoria_t *dados,sfMusic *music)
{
    sfMusic_destroy(music);

    if(dados->fase == 1)
    {
        music;
        music = sfMusic_createFromFile("musica/musica.ogg");
    }
    if(dados->fase == 2)
    {
        music;
        music = sfMusic_createFromFile("musica/fase2.ogg");
    }

    sfMusic_play(music);
}

void carregaPersonagem(sfSprite  *desenho)
{
    /*criacao do sprite e textura*/
    sfTexture *textura = sfTexture_createFromFile("imagens/personagens/personagens30.png", NULL);
    sfSprite_setTexture(desenho, textura, 0);
    /*criacao do sprite e textura*/
    /*escala do desenho*/
    sfVector2f fatorZoom;
    fatorZoom.x = 0.40f;
    fatorZoom.y = 0.40f;
    sfSprite_scale( desenho, fatorZoom);
    /*escala do desenho*/
}

void carregaCenario(sfSprite  *cenario, memoria_t *dados){
        sfTexture *cenario2;
        if(dados->fase == 1)  cenario2 = sfTexture_createFromFile("imagens/cenario/cenarion2.png", NULL);
        if(dados->fase == 2)  cenario2 = sfTexture_createFromFile("imagens/cenario/cenarion1.png", NULL);
        if(dados->fase == 0)  cenario2 = sfTexture_createFromFile("imagens/cenario/menu.png", NULL);
        if(dados->fase == 98) cenario2 = sfTexture_createFromFile("imagens/cenario/fim.png", NULL);
        if(dados->fase == 99) cenario2 = sfTexture_createFromFile("imagens/cenario/gameover.png", NULL);
        sfSprite_setTexture(cenario, cenario2, 0);
}

void carregaDadosCompartilhados(memoria_t *dados)
{
    dados->vida         = 5;
    dados->rifeiroSome  = 0;
    dados->rifeiroTempo = 0;
    dados->esteroides   = 0;
    dados->chocolate    = 0;
    dados->tiara        = 0;
    dados->detector     = 0;
    dados->pontuacao    = 0;
    dados->conhecimento = 0;
    dados->fase         = 0;
    dados->chocolateAtivo = 0;
    dados->esteroidesAtivo = 0;
    dados->tiaraAtivo = 0;
    dados->defesa = 0.00f;
    dados->maisVelTexugo= 0.00f;
    dados->maisVelInimigo= 0.00f;
    dados->maisVelInimigo= 0.00f;
    dados->quantidadecachorro      =5;
    dados->maiscachorro            =0;
    dados->quantidaderifeiro       =1;
    dados->maisrifeiro             =0;
    dados->quantidadepanda         =3;
    dados->quantidadedetector      =2;
    dados->quantidadechocolate     =5;
    dados->quantidadetiara         =1;
    dados->quantidadeesteiroides   =1;
    dados->quantidadeconhecimento  =10;
    dados->bloqueioBotao=0;
    dados->bloqueioVida=0;
    dados->tempochocolate =0;
    dados->tempoesteroides=0;
    dados->tempotiara=0;
    dados->tela =0;
    dados->posicao = 0;
    dados->jogando =1;
}

void carregaTexugo(personagem_t *texugo){
    texugo->posX  = 0;
    texugo->posY  = 0;
    texugo->tamX  = 25;
    texugo->tamY  = 25;
    texugo->idxQuadroTexugo = 0;
    texugo->idxQuadroTexugoMin = 0;
    texugo->idxQuadroTexugoMax = 1;
    texugo->idxQuadroTexugoMinOriginal =0;
    texugo->idxQuadroTexugoMaxOriginal =1;
    texugo->vel = 0.15f;
}

int main()
{

    /*criando uma janela*/
    sfRenderWindow *janela;
    sfVideoMode modoVideo = {RESOLUCAOALT, (RESOLUCAOLAG+RESOLUCAOMENU), 32};
    janela = sfRenderWindow_create(modoVideo,"Projeto Texugo !", sfResize | sfClose, NULL);
    /*criando uma janela*/

    /*
    Dados Compartilhados
    */
    struct memoria_t *dados;
    dados = (struct memoria_t*)malloc(sizeof(struct memoria_t));
    carregaDadosCompartilhados(dados);

    while( sfRenderWindow_isOpen( janela ) )
    {
        sfEvent evento;
        while( sfRenderWindow_pollEvent(janela, &evento))
        {
            if(evento.type == sfEvtClosed)
            {
                sfRenderWindow_close( janela );
            }

        }

        /*fonte*/
        sfFont* font;
        sfText* text;
        font = sfFont_createFromFile("fonte/arial.ttf");
        text = sfText_create();
        sfText_setFont(text, font);
        sfText_setCharacterSize(text, 22);
        /*fonte*/

        sfSprite  *desenho = sfSprite_create();
        carregaPersonagem(desenho);

        sfSprite  *cenario = sfSprite_create();
        carregaCenario(cenario,dados);

        /*musica
        sfMusic* music;
        musica(dados,music);
        /*musica*/

        /*Criar personagens*/
        srand(time(NULL));
        struct personagem_t *texugo;
        texugo = (struct personagem_t*)malloc(sizeof(struct personagem_t));
        carregaTexugo(texugo);

        struct inimigo_t *cachorro[(dados->quantidadecachorro+dados->maiscachorro)];
        for(int i=0; i<(dados->quantidadecachorro+dados->maiscachorro); i++)
        {
            cachorro[i] = (struct inimigo_t*)malloc(sizeof(struct inimigo_t));
            cachorro[i]->vida = -2;
            cachorro[i]->idxQuadroInimigoMin = 2;
            cachorro[i]->idxQuadroInimigo = cachorro[i]->idxQuadroInimigoMin;
            cachorro[i]->idxQuadroInimigoMax = 3;
            cachorro[i]->posX = (rand() % 900) + 50;
            cachorro[i]->posY = rand() % (700 - 100);
            cachorro[i]->sobe = 1;
            cachorro[i]->vel = 0.09f;
            cachorro[i]->tamX = 25;
            cachorro[i]->tamY = 25;
        }
        struct inimigo_t *rifeiro[(dados->quantidaderifeiro+dados->maisrifeiro)];
        for(int i=0; i<(dados->quantidaderifeiro+dados->maisrifeiro); i++)
        {
            rifeiro[i] = (struct inimigo_t*)malloc(sizeof(struct inimigo_t));
            rifeiro[i]->vida = -1;
            rifeiro[i]->idxQuadroInimigoMin = 4;
            rifeiro[i]->idxQuadroInimigo = 4;
            rifeiro[i]->idxQuadroInimigoMax = 5;
            rifeiro[i]->posX  = (rand() % 800) + 100;
            rifeiro[i]->posY  = rand() % (700 - 100);
            rifeiro[i]->vel = 0.05f;
            rifeiro[i]->tamX = 25;
            rifeiro[i]->tamY = 25;
        }

        struct inimigo_t *panda[dados->quantidadepanda];
        for(int i=0; i<dados->quantidadepanda; i++)
        {
            panda[i] = (struct inimigo_t*)malloc(sizeof(struct inimigo_t));
            panda[i]->vida = -999;
            panda[i]->idxQuadroInimigoMin = 6;
            panda[i]->idxQuadroInimigo = panda[i]->idxQuadroInimigoMin;
            panda[i]->idxQuadroInimigoMax = 7;
            panda[i]->posX = (rand() % 800) + 100;
            panda[i]->posY = rand() % (700 - 126);
            panda[i]->sobe = 1;
            panda[i]->vel = 0.08f;
            panda[i]->tamX = 25;
            panda[i]->tamY = 25;
        }
        /*Criar personagens*/

        /*Criar poderes*/

        poderes_t *detector[dados->quantidadedetector];
        for(int i=0; i<dados->quantidadedetector; i++)
        {
            detector[i] = (struct poderes_t*)malloc(sizeof(struct poderes_t));
            detector[i]->poder = 4;
            detector[i]->idxQuadro = 9;
            detector[i]->idxQuadroTexugoMin = 13;
            detector[i]->idxQuadroTexugoMax = 14;
            detector[i]->posX = (rand() % 800) + 100;
            detector[i]->posY = rand() % (700- 100);
            detector[i]->tamX  = 30;
            detector[i]->tamY  = 30;
            detector[i]->some = 0;
            if(dados->fase == 2){
                detector[i]->some = 1;
            }
            detector[i]->tempo = 7000;
            detector[i]->pontuacao = 150; //300
        }

        poderes_t *chocolate[dados->quantidadechocolate];
        for(int i=0; i<dados->quantidadechocolate; i++)
        {
            chocolate[i] = (struct poderes_t*)malloc(sizeof(struct poderes_t));
            chocolate[i]->poder = 1;
            chocolate[i]->idxQuadro = 10;
            chocolate[i]->idxQuadroTexugoMin = 17;
            chocolate[i]->idxQuadroTexugoMax = 18;
            chocolate[i]->posX = (rand() % 800) + 100;
            chocolate[i]->posY = rand() % (700- 100);
            chocolate[i]->tempo = 10000;
            chocolate[i]->tamX  = 30;
            chocolate[i]->tamY  = 30;
            chocolate[i]->some = 0;
            if(dados->fase == 2){
                chocolate[i]->some = 1;
            }
            chocolate[i]->pontuacao = 75;  //375 = 325
        }

        poderes_t *tiara[dados->quantidadetiara];
        for(int i=0; i<dados->quantidadetiara; i++)
        {
            tiara[i] = (struct poderes_t*)malloc(sizeof(struct poderes_t));
            tiara[i]->poder = 2;
            tiara[i]->idxQuadro = 8;
            tiara[i]->idxQuadroTexugoMin = 15;
            tiara[i]->idxQuadroTexugoMax = 16;
            tiara[i]->posX = (rand() % 800) + 100;
            tiara[i]->posY = rand() % (700- 100);
            tiara[i]->tempo = 7000;
            tiara[i]->tamX  = 30;
            tiara[i]->tamY  = 30;
            tiara[i]->some = 0;
            if(dados->fase == 2){
                tiara[i]->some = 1;
            }
            tiara[i]->pontuacao = 163;
        }

        poderes_t *piramide;
        piramide = (struct poderes_t*)malloc(sizeof(struct poderes_t));
        piramide->posX = rand() % (800- 100);
        piramide->posY = rand() % (700- 100);
        piramide->idxQuadro = 22;
        piramide->pontuacao = 0;
        piramide->tamX = 25;
        piramide->tamY = 25;
        piramide->some = 3;

        poderes_t *esteroides[dados->quantidadeesteiroides];
        for(int i=0; i<dados->quantidadeesteiroides; i++)
        {
            esteroides[i] = (struct poderes_t*)malloc(sizeof(struct poderes_t));
            esteroides[i]->poder = 3;
            esteroides[i]->idxQuadro = 19;
            esteroides[i]->idxQuadroTexugoMin = 11;
            esteroides[i]->idxQuadroTexugoMax = 12;
            esteroides[i]->idxQuadroTexugoMin2 = 20;
            esteroides[i]->idxQuadroTexugoMax2 = 21;
            esteroides[i]->posX = (rand() % 800) + 100;
            esteroides[i]->posY = rand() % (700- 100);
            esteroides[i]->tempo = 10000;
            esteroides[i]->tamX  = 30;
            esteroides[i]->tamY  = 30;
            esteroides[i]->some = 0;
            if(dados->fase == 2){
                esteroides[i]->some = 1;
            }
            esteroides[i]->pontuacao = 162;
        }

        poderes_t *conhecimento[dados->quantidadeconhecimento];
        for(int i=0; i<dados->quantidadeconhecimento; i++)
        {
            conhecimento[i] = (struct poderes_t*)malloc(sizeof(struct poderes_t));
            conhecimento[i]->idxQuadro = 23;
            conhecimento[i]->some = 0;
            conhecimento[i]->tamX = 25;
            conhecimento[i]->tamY = 25;
            conhecimento[i]->posX = (rand() % 800) + 100;
            conhecimento[i]->posY = rand() % (700- 100);
            conhecimento[i]->pontuacao = 100;
        }

        /*Criar poderes*/

        sfClock *relogio = sfClock_create(); //dano
        sfClock *relogioMove = sfClock_create(); //movimentação texugo, troca sprites
        sfClock *relogioFase2 = sfClock_create(); //para aumentar velocidades

        dados->jogando=1;

        while( dados->jogando & sfRenderWindow_isOpen( janela ) )
        {

            sfEvent evento;
            while( sfRenderWindow_pollEvent(janela, &evento))
            {
                if(evento.type == sfEvtClosed)
                {
                    sfRenderWindow_close( janela );
                }

            }
            //para exibir telas de menu, game over e fim
            while(dados->fase != 1 & dados->fase != 2)
            {
                    sfRenderWindow_clear( janela, sfBlack);
                    sfRenderWindow_drawSprite( janela, cenario,NULL);
                    sfRenderWindow_display ( janela );
                    if(sfKeyboard_isKeyPressed( sfKeyEnter )){
                        carregaDadosCompartilhados(dados);
                        dados->fase =1;
                        dados->jogando =0;
                    }
            }

            // relogio para atualizar o desenho
            sfTime tempo = sfClock_getElapsedTime( relogio );
            if( tempo.microseconds >= 2000000)
            {
                dados->dano = 0;
                dados->bloqueioBotao = 0;
                dados->posicao++;
                if(dados->posicao >= 2 ) {
                    dados->posicao =0;
                }
                sfClock_restart( relogio );
            }

            sfTime tempoMove = sfClock_getElapsedTime( relogioMove );
            if( tempoMove.microseconds >= 500000)
            {

                dados->posicao++;
                if(dados->posicao >= 2 ) {
                    dados->posicao =0;

                }
                sfClock_restart( relogioMove );
            }
            // movimentação do texugo
            if(sfKeyboard_isKeyPressed( sfKeyDown ) & texugo->posY <= (RESOLUCAOLAG-60.0f)) texugo->posY += texugo->vel;
            if(sfKeyboard_isKeyPressed( sfKeyUp )   & texugo->posY >= 0)   texugo->posY -= texugo->vel;
            if(sfKeyboard_isKeyPressed( sfKeyLeft ) & texugo->posX >= 0)   texugo->posX -= texugo->vel;
            if(sfKeyboard_isKeyPressed( sfKeyRight )& texugo->posX <= RESOLUCAOALT-60.0f) texugo->posX += texugo->vel;
            //poderes especiais
            if(sfKeyboard_isKeyPressed( sfKeyR ) & dados->rifeiroSome == 0)
            {
                ativaDetector(detector[0], dados, texugo);
            }

            if(sfKeyboard_isKeyPressed( sfKeyC ) & dados->chocolateAtivo == 0)
            {
               ativaChocolate(chocolate[0], dados, texugo);
            }

            if(dados->chocolateAtivo ==1)
            {
                inativaChocolate(chocolate[0], dados, texugo);
            }

            if(sfKeyboard_isKeyPressed( sfKeyE ) & dados->esteroidesAtivo == 0)
            {
                ativaEsteroides(esteroides[0], dados, texugo);
            }

            if(dados->esteroidesAtivo != 0){
                inativaEsteroides(esteroides[0], dados, texugo);
            }
            if(sfKeyboard_isKeyPressed( sfKeyT ) & dados->tiaraAtivo == 0)
            {
                ativaTiara(tiara[0], dados, texugo);
            }

            if(dados->tiaraAtivo ==1 )
            {
                inativaTiara(tiara[0], dados, texugo);
            }

            sfRenderWindow_clear( janela, sfBlack);
            sfRenderWindow_drawSprite( janela, cenario,NULL);
            caminhaTexugo(texugo, desenho, janela, dados);

            char informacao[200];
            sprintf(informacao, "Vidas %d Chocolate %d Tiara %d Detector Rifeiros %d   Esteroides %d  Conhecimento %d  :::: Pontuacao %d ", (int)round(dados->vida), dados->chocolate, dados->tiara, dados->detector, dados->esteroides, dados->conhecimento, dados->pontuacao);
            sfText_setString(text, informacao);
            sfText_setPosition(text,(sfVector2f){0.0f, 785.0f});
            sfRenderWindow_drawText(janela, text, NULL);
            /*inserção do poder na fase*/
            for(int i=0; i<dados->quantidadechocolate; i++)
            {
                if(!chocolate[i]->some)
                {
                    inserePoder(chocolate[i], desenho,janela);
                    if(colisaoPoder(texugo, chocolate[i]))
                    {
                        chocolate[i]->some = 1 ;
                        dados->chocolate++;
                        dados->pontuacao += chocolate[i]->pontuacao;
                    }
                }
            }

            for(int i=0; i<dados->quantidadedetector; i++)
            {
                if(!detector[i]->some)
                {
                    inserePoder(detector[i], desenho,janela);
                    if(colisaoPoder(texugo, detector[i]))
                    {
                        detector[i]->some = 1 ;
                        dados->detector++;
                        dados->pontuacao += detector[i]->pontuacao;
                    }
                }
            }

            for(int i=0; i<dados->quantidadetiara; i++)
            {
                if(!tiara[i]->some)
                {
                    inserePoder(tiara[i], desenho,janela);
                    if(colisaoPoder(texugo, tiara[i]))
                    {
                        tiara[i]->some = 1 ;
                        dados->tiara++;
                        dados->pontuacao += tiara[i]->pontuacao;
                    }
                }
            }

            for(int i=0; i<dados->quantidadeesteiroides; i++)
            {
                if(!esteroides[i]->some)
                {
                    inserePoder(esteroides[i], desenho,janela);
                    if(colisaoPoder(texugo, esteroides[i]))
                    {
                        esteroides[i]->some = 1 ;
                        dados->esteroides++;
                        dados->pontuacao += esteroides[i]->pontuacao;
                    }
                }
            }
            //chegou no fim da fase, exibe piramide para ir para segunda fase
            if(dados->pontuacao >= 1000 & dados->fase == 1 )
            {
                inserePoder(piramide, desenho,janela);
                if(colisaoPoder(texugo, piramide))
                {
                    piramide->some = 1 ;
                }
            }

            if(dados->fase == 2)
            {
                if(dados->conhecimento >= 5 && (texugo->idxQuadroTexugo == 0 || texugo->idxQuadroTexugo == 1)){
                    texugo->idxQuadroTexugo    = 24;
                    texugo->idxQuadroTexugoMin = 24;
                    texugo->idxQuadroTexugoMax = 25;
                }
                if(dados->conhecimento >= 10)
                {
                    dados->fase = 98;
                    dados->jogando     = 0;
                }
                for(int i=0; i<dados->quantidadeconhecimento; i++)
                {
                    if(!conhecimento[i]->some)
                    {
                        inserePoder(conhecimento[i], desenho,janela);
                        if(colisaoPoder(texugo, conhecimento[i]))
                        {
                            conhecimento[i]->some = 1 ;
                            dados->conhecimento++;
                            dados->pontuacao += conhecimento[i]->pontuacao;
                        }
                    }
                }
                sfTime tempoFase2 = sfClock_getElapsedTime( relogioFase2 );
                if( tempoFase2.microseconds >= 10000000)
                {
                    texugo->vel += 0.05f;
                    dados->maisVelInimigo += 0.03f;
                    sfClock_restart( relogioFase2 );

                }

            }

            //faz inimigos caminhar na fase
            for(int i=0; i<(dados->quantidadecachorro+dados->maiscachorro); i++)
            {
                caminhaCachorro(cachorro[i], texugo, dados, desenho,janela);
            }


            for(int i=0; i<dados->quantidadepanda; i++)
            {
                caminhaPanda(panda[i], texugo, dados, desenho,janela);
            }

            for(int i=0; i<(dados->quantidaderifeiro+dados->maisrifeiro); i++)
            {
                    caminhaRifeiro(rifeiro[i], texugo, dados, desenho, janela, detector[0]);
            }

            sfRenderWindow_display ( janela );

            if(dados->vida <= 0){
                dados->fase = 99;
                dados->jogando = 0;
                texugo->vel = 0.09f;
                dados->maisVelInimigo = 0.00f;
            }

            if(piramide->some == 1 & dados->pontuacao >= 1000 & dados->fase == 1 )
            {
                dados->jogando             = 0;
                dados->fase                = 2;
                dados->maiscachorro        = 4;
                dados->maisrifeiro         = 1;
                dados->quantidadepanda     = 0;
                dados->quantidadedetector  = 1;
                dados->quantidadechocolate = 1;
                dados->quantidadetiara     = 1;
                dados->quantidadeesteiroides   = 1;
                dados->quantidadeconhecimento  =10;
                texugo->idxQuadroTexugo = 0;
                texugo->idxQuadroTexugoMin = 0;
                texugo->idxQuadroTexugoMax = 1;
                dados->tempochocolate =0;
                dados->tempoesteroides=0;
                dados->tempotiara=0;
                dados->rifeiroTempo=0;

            }


        }
    }

    return 0;
}
