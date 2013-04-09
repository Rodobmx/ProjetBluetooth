# ============================================================================ #
# Nom du fichier : main.py                                                     #
#                                                                              #
# Auteur : ST. (KevinM)                                                        #
#                                                                              #
# Date de modifications : 21/03/2013                                           #
#                                                                              #
# Fichiers requis :                                                            #
# 	- gamepad.py                                                               #
#                                                                              #
# Commentaires :                                                               #
# 	                                                                           #
#                                                                              #
# ============================================================================ #

#!C:\Program Files\Python33\python.exe
# -*-coding:Utf-8 -*


# Importation des bibliotheques necessaires
import os, sys # On importe le module os qui dispose de variables 
          	   # et de fonctions utiles pour dialoguer avec votre 
          	   # systeme d'exploitation
import pygame
from pygame.locals import *

from package.gamepad import *


# Run
def main():

	running = True						# Variable qui continue la boucle si = True, stoppe si = False

	#Initialisation de la bibliotheque Pygame
	pygame.init()

	#Creation de la fenetre
	#fenetre = pygame.display.set_mode((640, 480))

	#On compte les joysticks
	nb_joysticks = pygame.joystick.get_count()
	if nb_joysticks < 1:
		print("! Aucun joystick de branche")
	else:
		mon_joystick = pygame.joystick.Joystick(0)	#Et on en cree un s'il y a en au moins un
		mon_joystick.init()							#Initialisation

		#Boucle infinie
		while running:
			for event in [pygame.event.wait() ] + pygame.event.get( ):	#On parcours la liste de tous les evenements recus
				if event.type == QUIT:     								#Si un de ces evenements est de type QUIT
					running = False   									#On arrete la boucle

				if event.type == JOYAXISMOTION:
					if event.axis == JOY_L_UD:
						print("joys_L_UD:{}".format(mon_joystick.get_axis(JOY_L_UD)))
					elif event.axis == JOY_L_RL:
						print("joys_L_RL:{}".format(mon_joystick.get_axis(JOY_L_RL)))
					elif event.axis == JOY_R_UD:
						print("joys_R_UD:{}".format(mon_joystick.get_axis(JOY_R_UD)))
					elif event.axis == JOY_R_RL:
						print("joys_R_RL:{}".format(mon_joystick.get_axis(JOY_R_RL)))

				if event.type == JOYBUTTONDOWN:
					if event.button == BUT_1:
						print("BUT_1")
					elif event.button == BUT_2:
						print("BUT_2")
					elif event.button == BUT_3:
						print("BUT_3")
					elif event.button == BUT_4:
						print("BUT_4")

					elif event.button == L_1:
						print("L_1")
					elif event.button == L_2:
						print("L_2")
					elif event.button == R_1:
						print("R_1")
					elif event.button == R_2:
						print("R_2")

					elif event.button == SEL:
						print("SEL")
						running = False

				# print("hat {}".format(mon_joystick.get_hat(0) ))
		quit()

# this runs the main function if this script is called to run.
#  If it is imported as a module, we don't run the main function.
if __name__ == "__main__": 
    main()


# Definitions
def quitt():
	fenetre.quit()
	mon_joystick.quit()
