
/*************************************************************************************/

// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>
#include <time.h>

#include <gl/gl.h>

#include <glut.h>

/*************************************************************************************/


// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)


struct Point { //Struktura przechowuj�ca wsp�rz�dne punkt�w 
	float x;
	float y;
};

struct Colour { //Struktura przechowuj�ca informacje o kolorze
	float R;
	float G;
	float B;
};

Colour newColour(Colour colour) { //Funkcja tworzenia losowego koloru
	
	colour.R = ((float)rand()) / RAND_MAX * 2.0 - 0.5;
	colour.G = ((float)rand()) / RAND_MAX * 2.0 - 0.5;
	colour.B = ((float)rand()) / RAND_MAX * 2.0 - 0.5;
	
	return colour;
}

Point perturbation(Point point) { //Funkcja losuj�ca zmiane w punkcie w ma�ym zakresie

	point.x= ((float)rand()) / RAND_MAX * 1.3 - 0.8;
	point.y= ((float)rand()) / RAND_MAX * 1.3 - 0.8;

	return point;
}

void square(float x, float y, float width){ //Funkcja rysuj�ca kwadrat
	
	Point a = { x,y }, b = { x,y }, c = { x,y }, d = { x,y };
	Colour colour{0.0f,0.0f,0.0f};
	
	a.x -= width / 2; //Obliczanie punkt�w kwadratu w zale�no�ci od jego �rodka i d�ugo�ci boku
	a.y += width / 2;
	b.x += width / 2;
	b.y += width / 2;
	c.x -= width / 2;
	c.y -= width / 2;
	d.x += width / 2;
	d.y -= width / 2;

	glBegin(GL_POLYGON);//Zainicjowanie okre�lania prymitywu poligon na osi
	colour = newColour(colour); //Losowanie koloru
	glColor3f(colour.R, colour.G, colour.B);
	glVertex2f(a.x, a.y);//Wstawianie punktu na osi

	colour = newColour(colour);
	glColor3f(colour.R, colour.G, colour.B);
	glVertex2f(c.x, c.y);

	colour = newColour(colour);
	glColor3f(colour.R, colour.G, colour.B);
	glVertex2f(d.x, d.y);

	colour = newColour(colour);
	glColor3f(colour.R, colour.G, colour.B);
	glVertex2f(b.x, b.y);
	glEnd();

}
void carpet(float x, float y, float width, int level) {//Funkcja do tworzenia dywanu
	
	Point rand = { x,y };
	rand = perturbation(rand);
	
	x += rand.x;//Zmiana koordynat�w �rodka przez losow� warto�� x i y
	y += rand.y;

	Point a = { x,y }, b = { x,y }, c = { x,y }, d = { x,y };

	if (level > 0) {

		a.x -= width / 6;//Obliczanie rog�w pomniejszego kwadratu
		a.y += width / 6;
		b.x += width / 6;
		b.y += width / 6;
		c.x -= width / 6;
		c.y -= width / 6;
		d.x += width / 6;
		d.y -= width / 6;
		
		carpet(x - width / 3, y + width / 3, width / 3, level - 1);//1 Wywo�anie rekurencji w ka�dym z 8 pozosta�ych kwadrat�w
		carpet(x            , y + width / 3, width / 3, level - 1);//2   --------------
		carpet(x + width / 3, y + width / 3, width / 3, level - 1);//3	|	1	2	3  |
		carpet(x - width / 3, y            , width / 3, level - 1);//4	|			   |
		carpet(x + width / 3, y            , width / 3, level - 1);//5	|   4   X   5  |
		carpet(x - width / 3, y - width / 3, width / 3, level - 1);//6	|              |
		carpet(x            , y - width / 3, width / 3, level - 1);//7	|   6   7   8  |
		carpet(x + width / 3, y - width / 3, width / 3, level - 1);//8	---------------
	} else square(x, y, width);//Gdy zejdziemy do porz�danego poziomu nast�puje rysowanie kwadratu


}
void RenderScene(void)

{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	carpet(0, 0, 180, 3);
	

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)

{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary


}

/*************************************************************************************/

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)



	if (horizontal <= vertical)

		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych     

	glLoadIdentity();

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void){
	srand(time(NULL));

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Dywan Sierpi�skiego");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/