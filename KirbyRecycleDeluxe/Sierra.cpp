#include "Sierra.h"
Sierra::Sierra() {
	objC = new Condor(1000, 500, 1, 1);
}
Sierra::~Sierra() {
	for (int i = 0; i < arrGr.size(); i++)
		delete arrGr.at(i);
	for (int i = 0; i < arrCsh.size(); i++)
		delete arrCsh.at(i);
	delete objC;
}
bool Sierra::Colision_Enemigos() {
	bool resultado = false;
	// Condor
	Rectangle r1 = objK->getRectangle();
	Rectangle r2 = objC->getRectangle();
	if (r1.IntersectsWith(r2)) {
		resultado = true;
		objK->setVidas();
		objK->setX(5);
		objK->setY(5);
	}
	// Granizo
	for (int i = 0; !resultado && i < arrGr.size(); i++) {
		Rectangle rgr = arrGr.at(i)->getRectangle();
		if (r1.IntersectsWith(rgr)) {
			resultado = true;
			time_pausa = time(0);
			objK->setVidas();
			objK->Volar(none);
			arrGr.at(i)->setEliminar(true);
			pausa = true;
		}
	}
	return resultado;
}
bool Sierra::Colision_PowerUp() {
	bool resultado = false;
	Rectangle r1 = objK->getRectangle();

	for (int i = 0; i < arrCsh.size(); i++) {
		Rectangle rch = arrCsh.at(i)->getRectangle();
		if (r1.IntersectsWith(rch)) {
			resultado = true;
			time_rapidez = time(0);
			rapidez = true;
			arrCsh.at(i)->setEliminar(true);
			// Empieza la velocidad
			time_rapidez = time(0);
			rapidez = true;
			objK->setFracMovX(2);
			objK->setFracMovY(2);
		}
	}
	return resultado;
}
void Sierra::Insertar_Enemigos() {
	// Granizo
	if (difftime(time(0), time_enemigos) > 5) {
		arrGr.push_back(new Granizo(rand() % 1400, rand() % 200 + 20, 200, 200));
		time_enemigos = time(0);
	}
}
void Sierra::Insertar_PowerUp(Graphics^ g, Bitmap^ bmp) {
	// Cushuro
	if (difftime(time(0), time_powerup) > 20) {
		arrCsh.push_back(new Cushuro((gcnew System::Random())->Next(10, g->VisibleClipBounds.Width - bmp->Width),
			(gcnew System::Random())->Next(20, 200), 1, 1));
		time_powerup = time(0);
	}
}
void Sierra::Dibujar_Kirby(Graphics^ g, Bitmap^ bmp, Bitmap^ bmpCong) {
	// Kirby Volando
	objK->Dibujar_Imagen(g, bmp);
	Colision_Basura();
	Colision_Enemigos();
	Colision_PowerUp();

	// Rapidez evaluada
	if (rapidez) {
		if (difftime(time(0), time_rapidez) > 5)
			rapidez = false;
		objK->setFracMovX(4);
		objK->setFracMovY(4);
	}
	objK->Mover(g);

	if (objK->getVidas() == 0)
		esFin = true;
	if (contador == 30) {
		esFin = true;
		// Hacer que pase el otro nivel
		Resumen(g);
	}

	// Si esta congelado 
	if (pausa)
		Dibujar_Congelado(g, bmpCong);
}
void Sierra::Dibujar_Enemigos(Graphics^ g, Bitmap^ bmpGr, Bitmap^ bmpC) {

	objC->Dibujar_Imagen(g, bmpC);
	objC->Mover(g);

	for (int i = 0; i < arrGr.size(); i++) {
		arrGr.at(i)->Dibujar_Imagen(g, bmpGr);
		arrGr.at(i)->Mover(g);
	}
	for (int i = 0; i < arrGr.size(); i++) {
		if (arrGr.at(i)->getEliminar())
			arrGr.erase(arrGr.begin() + i);
	}
}
void Sierra::Dibujar_PowerUp(Graphics^ g, Bitmap^ bmp) {
	for (int i = 0; i < arrCsh.size(); i++) {
		arrCsh.at(i)->Dibujar(g, bmp);
		arrCsh.at(i)->Mover(g);
	}
}
void Sierra::Dibujar_Congelado(Graphics^ g, Bitmap^ congelado) {
	objK->Dibujar_Congelado(g, congelado);
}
