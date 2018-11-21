
/*************************************************************************************/

// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>
#include <time.h>

#include <gl/gl.h>

#include <glut.h>

/*************************************************************************************/


// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)


struct Point { //Struktura przechowuj¹ca wspó³rzêdne punktów 
	float x;
	float y;
};

struct Colour { //Struktura przechowuj¹ca informacje o kolorze
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

Point perturbation(Point point) { //Funkcja losuj¹ca zmiane w punkcie w ma³ym zakresie

	point.x= ((float)rand()) / RAND_MAX * 1.3 - 0.8;
	point.y= ((float)rand()) / RAND_MAX * 1.3 - 0.8;

	return point;
}

void square(float x, float y, float width){ //Funkcja rysuj¹ca kwadrat
	
	Point a = { x,y }, b = { x,y }, c = { x,y }, d = { x,y };
	Colour colour{0.0f,0.0f,0.0f};
	
	a.x -= width / 2; //Obliczanie punktów kwadratu w zale¿noœci od jego œrodka i d³ugoœci boku
	a.y += width / 2;
	b.x += width / 2;
	b.y += width / 2;
	c.x -= width / 2;
	c.y -= width / 2;
	d.x += width / 2;
	d.y -= width / 2;

	glBegin(GL_POLYGON);//Zainicjowanie okreœlania prymitywu poligon na osi
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
	
	x += rand.x;//Zmiana koordynatów œrodka przez losow¹ wartoœæ x i y
	y += rand.y;

	Point a = { x,y }, b = { x,y }, c = { x,y }, d = { x,y };

	if (level > 0) {

		a.x -= width / 6;//Obliczanie rogów pomniejszego kwadratu
		a.y += width / 6;
		b.x += width / 6;
		b.y += width / 6;
		c.x -= width / 6;
		c.y -= width / 6;
		d.x += width / 6;
		d.y -= width / 6;
		
		carpet(x - width / 3, y + width / 3, width / 3, level - 1);//1 Wywo³anie rekurencji w ka¿dym z 8 pozosta³ych kwadratów
		carpet(x            , y + width / 3, width / 3, level - 1);//2   --------------
		carpet(x + width / 3, y + width / 3, width / 3, level - 1);//3	|	1	2	3  |
		carpet(x - width / 3, y            , width / 3, level - 1);//4	|			   |
		carpet(x + width / 3, y            , width / 3, level - 1);//5	|   4   X   5  |
		carpet(x - width / 3, y - width / 3, width / 3, level - 1);//6	|              |
		carpet(x            , y - width / 3, width / 3, level - 1);//7	|   6   7   8  |
		carpet(x + width / 3, y - width / 3, width / 3, level - 1);//8	---------------
	} else square(x, y, width);//Gdy zejdziemy do porz¹danego poziomu nastêpuje rysowanie kwadratu


}
void RenderScene(void)

{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	carpet(0, 0, 180, 3);
	

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)

{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnêtrza okna - ustawiono na szary


}

/*************************************************************************************/

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreœlenie okna obserwatora.
	// Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
	// Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)



	if (horizontal <= vertical)

		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych     

	glLoadIdentity();

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



void main(void){
	srand(time(NULL));

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Dywan Sierpiñskiego");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/