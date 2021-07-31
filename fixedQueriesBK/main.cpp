#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include "resource.h"
HINSTANCE hInst;
using namespace std;
int EditDist(string& a,string& b) // Distancia de Edicion // Prog dinamica O(m*n)
{   int m = a.length(), n = b.length();
    int dp[m+1][n+1]={{0}};
    //vector < vector <int> > dp(m+1, vector<int>(n+1, 0));
    for (int i=0; i<=m; i++) // llenando casos base
        dp[i][0] = i;
    for (int j=0; j<=n; j++)
        dp[0][j] = j;
    for (int i=1; i<=m; i++) // llenar matriz prog dinamica
    {   for (int j=1; j<=n; j++)
        {   if (a[i-1] != b[j-1])
                dp[i][j] = min(1+dp[i-1][j],     //borrar
                            min(1 + dp[i][j-1],  //insertar
                             1 + dp[i-1][j-1])); //reemplazar
            else
                dp[i][j] = dp[i-1][j-1];
        }
    }
    return dp[m][n];
}
class Nodo // clase nodo que almacenara una palabra y un map de nodos hijos.
{   public:
    string Palabra;
    map <int,Nodo *> Sig;
    // para indicar el nodo hijo que es llave del nivel;
    // int key = -1;
    Nodo(string P="")
    {   Palabra=P;
    }
};
class ArbolBK // clase arbol
{   public:
    Nodo *Raiz; // declarando la raiz del arbol
    map<int,string> key_per_level; // vector para indicar  la palabra clave para cada nivel;
    ArbolBK()
    {   Raiz=NULL; // inicialmente el arbol tendra una raiz null, lo mismo a decir que aun no tiene palabras
    }
    void InsertarBloque(string S) // insercion de un bloque de palabras al arbol
    {   string token; // identificador de palabras del bloque
        stringstream cins(S); // tratamos al bloque S como una entrada similar a cin >>;
        while(getline(cins,token,' ')) // hacemos una iteracion del stream separados por un espacio " "
            Insertar(token); // insertamos la palabra al arbol;
    }
    void Insertar(string S)
    {   if(Raiz==NULL){ // si el arbol esta vacio entonces creamos el nodo raiz con la palabra S;
            Raiz=new Nodo(S);
            key_per_level[0] = S;
            Raiz->Sig[0] = new Nodo(S);
        }

        else
            Ins(Raiz,S,0); // en caso que el arbol tenga almenos un nodo, evaluamos la palabra empezando con la raiz;
    }
    void Ins(Nodo *&R,string S, int level) // level indica en nivel que estamos comparando;
    {   int dist=EditDist(R->Palabra,S); // calculamos la distancia de la palabra del nodo actual con la palabra que queremos insertar
        if(dist>0) // si la palabras esdistinta entonces continuamos con la operacion
        {   if((R->Sig).find(dist)==(R->Sig).end()) // buscamos si ya existe un nodo hijo que tenga la misma distancia que la palabra S
                (R->Sig)[dist]=new Nodo(S); // si no existe un nodo con la misma distancia entonces agregamos el nodo(palabra) como hijo del nodo actual;
            else{
                if(key_per_level[level+1] == ""){ // si aun no existe el key en ese nivel;
                        key_per_level[level+1] = R->Sig[dist]->Palabra; // map level string;
                        R->Sig[dist]->Sig[0] =  new Nodo(R->Sig[dist]->Palabra);
                        Ins((R->Sig)[dist],S,level+1);
                }else{ // si ya existe un key;
                    string nodo_p = R->Sig[dist]->Palabra;
                    //int dist_nodo_key = EditDist(R->Sig[dist]->Palabra,key_per_level[level+1]);
                    R->Sig[dist]->Palabra = key_per_level[level+1];
                    Ins(R->Sig[dist],nodo_p,level+1);
                    Ins((R->Sig)[dist],S,level+1);
                }
                //Ins((R->Sig)[dist],S); // en caso que el nodo tenga un hijo con la misma distancia entonces reevaluamos la palabra con ese nodo;
            }
                // hijo repetido.

        }
    }
    void Mostrar(long T) // mostrar por CLI
    {   if(Raiz!=NULL)
            Mos(Raiz,T); // mostramos los nodos siempre que exista almenos uno;
        cout<<endl;
    }
    void Mos(Nodo *R,long T) // T = distancia del nodo anterior con el nodo actual;
    {   cout<<T<<" - "<<R->Palabra<<endl;  // imprimiendo la palabra del nodo actual
        map <int,Nodo *>::iterator it;
        for(it=(R->Sig).begin();it!=(R->Sig).end();it++) // comprobamos que el nodo tiene nodos hijo;
            Mos((*it).second,T*10+(*it).first); // si el nodo tiene hijos entonces llamamos a la misma funcion recursivamente
    }
    void MostrarW(HDC hdc,int x,int y,int a, int l) // a = ancho que se puede usar para dibujar
    {   if(Raiz!=NULL)
            MosW(hdc,Raiz,x,y,a,l); // mostramos el arbol siempre que tenga almenos una hoja

    }
    void MosW(HDC hdc,Nodo *R,int x,int y,int a,int l) // single node;
    {   map <int,Nodo *>::iterator it; // iterador de los nodos hijos.
        /*
            ii = contador de cantidad de hijos por nodo;
            ancho = ancho destinado por cada hijo del nodo;
            tam = cantidad de hijos del nodo actual;
        */
        int ii,ancho,tam=(R->Sig).size();
        if(tam>0) // siempre que el nodo tenga hijos
        {   ancho=a/tam; // dividimos equitativamente el ancho para cada nodo hijo;
            for(ii=0,it=(R->Sig).begin();it!=(R->Sig).end();it++,ii++) // iteramos it para cada nodo hijo
            {   MoveToEx(hdc,x,y,NULL); // movemos las coordenadas de hcn para empezar a trazar las lineas;
                LineTo(hdc,x+ii*ancho-a/2,y+100); // trazando la linea que apunta a cada nodo hijo;
                MosW(hdc,(*it).second,x+ii*ancho-a/2,y+100,ancho-15,l+1); // recursividad para los nodos hijos;
                char Cad2[10]; // declarando una cadena que almacenada la palabra del nodo actual;
                sprintf(Cad2,"%i",(*it).first); // copiamos la distancia de el nodo con sus hijos a la cadena (char)Cad2
//                if ((*it).second->Palabra == key_per_level[l+1]){
//                    cout << R->Palabra << "\t"<< key_per_level[l+1] << endl;
//                    char key_message[] = "key-> ";
//                    TextOut(hdc,x+ii*ancho-a/2-10,y+80,key_message,strlen(key_message));
//                }

                TextOut(hdc,x+ii*ancho-a/2-10,y+100-5,Cad2,strlen(Cad2)); // escribimos esa distancia al nodo (graficamente);
            }
        }
        char Cad[2]=" ";
        for(ii=0;ii<strlen((R->Palabra).c_str());ii++) // iteramos por cada caracter de la palabra del nodo;
        {   Cad[0]=((R->Palabra).c_str())[ii]; // copiamos ese caracter al inicio de Cad 2; ejm [p]erro -> "p ";
            TextOut(hdc,x+1,y+1+ii*14,Cad,1); // escribimos cad 2 en el cuadro de texto de resultados;
        }
        if (R->Palabra == key_per_level[l]){
            cout << R->Palabra << "\t"<< key_per_level[l] << endl;
            char key_message[] = "[key]";
            TextOut(hdc,x+1,y-15,key_message,strlen(key_message)); // escribimos cad 2 en el cuadro de texto de resultados;

        }
    }
    vector <string> similar(Nodo *R,string S,int Tol) // vector que almacena los resultados
    {   int dist, var; // distancia; var: rango de evaluacion de distancias de hijos;
        vector <string> ret; // vector que almacena resultados | palabras similares
        if(R!=NULL) // siempre que el arbol no este vacio
        {   dist=EditDist(R->Palabra,S); // calculamos la distancia de la palabra S con el nodo/raiz
            if(dist<=Tol)                   // si es menorigual que la tolerancia
                ret.push_back(R->Palabra);  //entonces agregamos esa palabra al vector de resultados;
            var=dist-Tol; // de la teoria [dist - tol, dis+tol]
            if(var<0)
                var=1; // si la operacion resulta en un numero negativo entonces la variacion empieza en 0
            vector <string> tmp; // vector que almacena resultados de nodos hijos (recursividad);
            for(;var<dist+Tol;var++)
            {   if((R->Sig).find(var)!=(R->Sig).end())// iteramos por los hijos del nodo actual;
                {   tmp=similar((R->Sig)[var],S,Tol); // Y buscamos la similaridad de la palabra a partir del nodo actual;
                    ret.insert(ret.end(),tmp.begin(),tmp.end()); // del vector resultado que nos devuelva la funcion similar,
                    // contatenamos ese vector al vector actual;
                }
            }
        }
        return ret; // devolvemos el vector resultado;
    }
};
ArbolBK A;
 //


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   switch(uMsg)
    {   case WM_PAINT: // evento dibujar
        {   PAINTSTRUCT ps;
            HDC hdc;
            hdc=BeginPaint(hwndDlg,&ps);
            A.MostrarW(hdc,650,50,800,0); // llamamos a la funcion dibujar arbol especificando el punto de inicio y el ancho a = 700
                                    // 0 indica el nivel desde donde empezar;
            A.Mostrar(1); // poco relevante // usado solo para iniciar con el dibujo del arbol;
            EndPaint(hwndDlg,&ps); //
        }
        case WM_INITDIALOG:
        {
        }
        return TRUE;
        case WM_CLOSE:
        {   EndDialog(hwndDlg, 0); // cerrando la ventana
        }
        return TRUE;
        case WM_COMMAND: //
        {   switch(LOWORD(wParam))
            {   case INSERTAR: // en caso que pulsemos el boton de [insertar]
                {   char Cad[500]; // el cuadro de texto tendra un maximo de 500 caracteres para insertar
                    GetDlgItemText(hwndDlg,PAL,Cad,500); // procesamos el texto del cuadro de entrada al array Cad;
                    A.InsertarBloque(Cad); // insertamos ese bloque de palabras al arbol;
                    InvalidateRect(hwndDlg,NULL,true); // redibujamos la ventana y causamos el evento WM_PAINT
                }
                return TRUE;
                case BUSCAR: // en caso que pulsemos el boton de [buscar];
                {   char Cad2[20]; // la longitud de la palabra tiene que ser menor a 20;
                    GetDlgItemText(hwndDlg,BUSQ,Cad2,20); // procesamos el texto del cuadro | palabra
                    int Tol=GetDlgItemInt(hwndDlg,TOL,NULL,NULL); // procesamos el texto del cuadro | tolerancia
                    vector <string> R=A.similar(A.Raiz,Cad2,Tol); // vector que almacenara las palabras similares llamando a la funcion;
                    stringstream ss; // ss funcionara como una entrada;
                    for(int i=0;i<R.size();i++) // iteramos por los resultados
                        ss<<R[i]<<" "; // imprimimos las palabras a ss;
                    SetDlgItemTextA(hwndDlg,RESUL,(ss.str()).c_str()); // tratamos la entrada ss como un array char*
                    // para finalmente imprimirlo en la ventana;
                }
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    A.Insertar("mesa");
    A.Insertar("masa");
    A.Insertar("casa");
    A.Insertar("misa");
    A.Insertar("rosa");

    hInst=hInstance; // instancia
    InitCommonControls(); // controles
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain); // procesador de eventos;
}
