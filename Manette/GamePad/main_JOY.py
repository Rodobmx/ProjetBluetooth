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

# ==============
# s : sens '+' ou '-'
# gg: vitesse roue gauche 0-99 en ascii
# dd: vitesse roue droite 0-99 en ascii
# v : vitesse
# l : led
# - : debut de trame
# ; : fin de trame


# -s; : arret d'urgence

# -v:sdd,sgg; : envoie de vitesse
# -v:+80,-80;

# -l:r,v; : allumer led
# r,v : 0 ou 1 (etein, allumé)
# ==============


#!C:\Program Files\Python33\python.exe
# -*-coding:Utf-8 -*


VERSION = "0.1"

# Importation des bibliotheques necessaires 
try:
	import os	# On importe le module os qui dispose de variables 
				# et de fonctions utiles pour dialoguer avec votre 
				# systeme d'exploitation
	import sys
	from sys import version_info

	import serial
	import pygame
	from pygame.locals import *

	from package.gamepad import *

except ImportError as err:
        print("Impossible de charger le module. {}".format(err))
        sys.exit()

# Run
def main():
	
	#Initialisation de la bibliotheque Pygame
	pygame.init()

	#On compte les joysticks
	nb_joysticks = pygame.joystick.get_count()
	if nb_joysticks < 1:
		print("! Aucune manette de branchée")
		print("  Veuillez brancher une manette et recommencer.")
		sys.exit()
	else:
		mon_joystick = pygame.joystick.Joystick(0)	#Et on en cree un s'il y a en au moins un
		mon_joystick.init()							#Initialisation

		ser = serial.Serial(
			port = "COM23",
			baudrate = 9600,
			parity = serial.PARITY_NONE,
			stopbits = serial.STOPBITS_ONE,
			bytesize = serial.EIGHTBITS
		)

		try:
			ser.open()
		except serial.SerialException as e:
			print("! Erreur lors de l'ouverture du port serie : " + str(e))
			
		if not ser.isOpen():
			print("! Le port serie n'est pas ouvert")
			sys.exit()
		else:
			print("  Port ouvert on peut envoyer qq chose")
			
			sendCmd("TEST", ser)


			joy_l_val = 0    	# la valeur du joystick de gauche
			joy_l_dir = '*'		# le signe  du joystick de gauche
			joy_l_str = '---'

			joy_r_val = 0    	# la valeur du joystick de droite
			joy_r_dir = '*'      # le signe  du joystick de droite
			joy_r_str = '---'

			led_l = 0           # la valeur pour l'état de la led de gauche
			led_r = 0           # la valeur pour l'état de la led de droite


			toSend = False		# boolean indiquant qu qq chose est à envoyer
			trame_to_send = ''  # la chaine de caractere qui sera transmise


			# ...
			clock = pygame.time.Clock()

			# Variable qui continue la boucle si = True, stoppe si = False
			running = True

			#Boucle infinie
			while running:
				for event in pygame.event.get( ):	#On parcours la liste de tous les evenements recus
					if event.type == QUIT:     								#Si un de ces evenements est de type QUIT
						running = False   									#On arrete la boucle

					if event.type == JOYBUTTONDOWN:
						if event.button == SEL:
							trame_to_send = "-s;"
							toSend = True
							running = False

						if event.button == L_1:
							led_l = '1'
							toSend = True
							
						if event.button == R_1:
							led_r = '1'
							toSend = True

						trame_to_send = "-l:" + str(led_l) + ',' + str(led_r) + ';'


					if event.type == JOYAXISMOTION:
						if event.axis == JOY_R_UD:
							if event.value < 0:
								joy_r_dir = '+'
							else:
								joy_r_dir = '-'

							joy_r_val = int(abs(mon_joystick.get_axis(JOY_R_UD) * 100))
							joy_r_str = convStrOn3Digits(joy_r_val)
							toSend = True

						if event.axis == JOY_L_UD:
							if event.value < 0:
								joy_l_dir = '+'
							else:
								joy_l_dir = '-'

							joy_l_val = int(abs(mon_joystick.get_axis(JOY_L_UD) * 100))
							joy_l_str = convStrOn3Digits(joy_l_val)
							toSend = True

						trame_to_send = "-v:" + str(joy_l_dir) + str(joy_l_str) + ',' + str(joy_r_dir) + str(joy_r_str) + ';'
						
					if toSend is True:
						sendCmd(trame_to_send, ser)
						toSend = False

				clock.tick(20)
			
			# running = False	
			ser.close()
		

# Definitions
def sendCmd(cmd, ser):
	"""
	Description : Transmet une chaine de caractères sur la liaison série.

	Param(s) : cmd, la chaine de caractères à envoyer
			   ser, ...

	Output : Nothing
	"""

	cmd += "\r\n"
	print(cmd)
	ser.write(cmd.encode('ascii'))


def convStrOn3Digits(value):
	"""
	Description : Converti une chaine de caractères, représentant un nb compris entre 0 et 100
				  en une chaine de caracteres, sur 3 digits

	Param(s) : value, la valeur à Convertir

	Output : Nothing
	"""

	string = ''

	if value > 999:
		return string
	else:
		if value < 10:
			string = '00' + str(value)
		elif value < 100:
			string = '0' + str(value)
		else:
			string = str(value)

		return string


# this runs the main function if this script is called to run.
#  If it is imported as a module, we don't run the main function.
if __name__ == "__main__":
    main()
