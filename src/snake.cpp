// #define SDL_MAIN_HANDLED
// #include <SDL2/SDL.h>
// #include <iostream>
// using namespace std;
// int main(int argc, char* argv[]) {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//         printf("SDL_Init Error: %s\n", SDL_GetError());
//         return 1;
//     }
    
//     SDL_Window* window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
//     if (window == NULL) {
//         printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
//         SDL_Quit();
//         return 1;
//     }
    
//     SDL_Delay(3000);  // Keep the window open for 3 seconds
    
//     SDL_DestroyWindow(window);
//     SDL_Quit();
    
//     return 0;
// }

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
using namespace std;
//#include <utility>

class node{
public:
node* next;
bool val;
int pos_x;
int pos_y;
node(bool val,int x,int y){next=nullptr;pos_x=x;pos_y=y;}

};

class snake{

node* head;
node* tail;

int random_x = rand() % 50;
int random_y = rand() % 50;
    
public:
int direction=4;
int score=0;


snake(){head=tail=new node(1,25,25);adder(3);}


void adder(int addable){
    for(int i=0;i<addable;i++){
        if(head==tail){
            node* newtail=new node(1,26,25);
            tail->next=newtail;
            tail=newtail;

        }else{
            int diff_x=lastbodydir_x();
            int diff_y=lastbodydir_y();
            node* newtail=new node(1,tail->pos_x+diff_x,tail->pos_y+diff_y);
            tail->next=newtail;
            tail=newtail;
           
        }

    }
}
int lastbodydir_x() {
    node* curr = head;
    while (curr != NULL && curr->next != tail) {
        curr = curr->next;
    }
    if (curr != NULL && curr->next == tail) {
        return tail->pos_x - curr->pos_x;
    } else {
        cout << "Can't use last body dir x" << endl;
        return 0;
    }
}

int lastbodydir_y() {
    node* curr = head;
    while (curr != NULL && curr->next != tail) {
        curr = curr->next;
    }
    if (curr != NULL && curr->next == tail) {
        return tail->pos_y - curr->pos_y;
    } else {
        cout << "Can't use last body dir y" << endl;
        return 0;
    }
}

void snakemove(){
    node* curr = head;
    int tmp_x = head->pos_x;
    int tmp_y = head->pos_y;
    int t2p_x, t2p_y;

    
    if (direction == 1) {  
        head->pos_y -= 1;
    } else if (direction == 2) {  
        head->pos_y += 1;
    } else if (direction == 3) {  
        head->pos_x += 1;
    } else if (direction == 4) {  
        head->pos_x -= 1;
    }

    
    curr = head->next;  
    while (curr != NULL) {
    
        t2p_x = curr->pos_x;
        t2p_y = curr->pos_y;

        curr->pos_x = tmp_x;
        curr->pos_y = tmp_y;

        tmp_x = t2p_x;
        tmp_y = t2p_y;

        curr = curr->next;
    }
}

void throwwall(){
    node* curr=head;
    while(curr!=NULL){
        if(curr->pos_x>49){curr->pos_x=0;}
        if(curr->pos_x<0){curr->pos_x=49;}
        if(curr->pos_y>49){curr->pos_y=0;}
        if(curr->pos_y<0){curr->pos_y=49;}
        curr=curr->next;
    }
}

int contact(){
    node* curr=head;
    node* cur=head;
    while(curr!=NULL){
        cur=head;
        while(cur!=NULL){
            if(cur!=curr){
                if((cur->pos_x==curr->pos_x)&&(cur->pos_y==curr->pos_y)){
                    return 1;
                }
            }
        cur=cur->next;
        }
    curr=curr->next;
    }
    return 0;
}

void snakeprinter(SDL_Renderer* renderer ){
    node* curr=head;
    
    while(curr!=NULL){
        if(curr==head){
            SDL_SetRenderDrawColor(renderer ,255,174,66,255);
        }else{
            SDL_SetRenderDrawColor(renderer ,255,255,255,255);
        }
        SDL_RenderDrawPoint(renderer , curr->pos_x, curr->pos_y);
        curr=curr->next;
    }
    SDL_SetRenderDrawColor(renderer ,255,0,0,255);
    SDL_RenderDrawPoint(renderer ,random_x,random_y);
}

void appleeat() {
    if(head->pos_x ==random_x && head->pos_y == random_y){
        adder(5);
        apple();
        cout<<endl;
        cout<<"new Apple Pos \nX="<<random_x<<"\nY="<<random_y<<endl;
        counterscore();
    }
}

void apple() {
    bool conflict = true;

    while (conflict) {
        random_x=rand() % 50;
        random_y=rand() % 50;

        node* curr=head;
        conflict=false;

        while (curr!=NULL) {
            if (random_x==curr->pos_x && random_y == curr->pos_y) {
                conflict=true;
                break;
            }
            curr = curr->next;
        }
    }
}

void counterscore(){
    score++;
} 


};


int main(){
snake s;



    SDL_Window* window=nullptr;
    SDL_Renderer* renderer=nullptr;   
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Snake ig?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000 , 1000, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(renderer, 50, 50);
    SDL_Event e;
    bool runing=true;
    string scoreText = "Score: " + to_string(s.score);
    
    while(runing){
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                runing = false;
                break;
            }else{
                if(e.type == SDL_KEYDOWN){
                    if(e.key.keysym.sym == SDLK_UP){
                        s.direction=1;
                    }else if(e.key.keysym.sym == SDLK_DOWN){
                        s.direction=2;
                    }else if(e.key.keysym.sym == SDLK_RIGHT){
                        s.direction=3;
                    }else if(e.key.keysym.sym == SDLK_LEFT){
                        s.direction=4;
                    }else if(e.key.keysym.sym == SDLK_k){
                        s.adder(20);
                    }



                }//for keydown
            }//for events

            }

        s.snakemove();
        s.throwwall();

        s.appleeat();


        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        s.snakeprinter(renderer);
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(75);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}