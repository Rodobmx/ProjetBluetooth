# ============================================================================ #
# Nom du fichier : testPWM.py                                                  #
#                                                                              #
# Auteur : ST. (KevinM)                                                        #
#                                                                              #
# Date de modifications : 14/05/2013                                           #
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


# -s; : arret durgence

# -v:sddd,sggg; : envoie de vitesse
# -v:+080,-080;

# -l:r,v; : allumer led
# r,v : 0 ou 1 (eteint, allume)
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
	import time
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

	
	# on cherche les ports com disponibles
	print("- Scan des ports serie en cours ...")
	list = scan_serial_ports()
	if len(list) < 1:
		print("! Aucun port serie détecte")
		sys.exit()
	else:
		print("  PORT SERIE OK")

	# on affiche les ports serie disponibles
	m=0
	print("  Ports series disponibles :")
	for s in list:
		print("    {:d} {!s}".format(m, s))
		m=m+1
		
	# on demande a lutilisateur de saisir le nom du port serie souhaité
	print("  Veuillez saisir le port port que vous voulez utiliser")
	# Verif saisie
	selected_port = -1
	while selected_port < 0 :
		selected_port = input("  > ")
		try:
			selected_port = int(selected_port)
		except ValueError:
			print("  ! Vous n'avez pas saisi de nombre")
			selected_port = -1
			continue
		if ((selected_port < 0) or (selected_port > 256)):
			print("  ! Vous avez saisi un port ds la limite des ports disponibles")
			selected_port = -1

	
	try:
		# Configuration et ouverture de la connexion
		ser = serial.Serial(
			port         = str(list[selected_port][1]),
			baudrate     = 9600,
			parity       = serial.PARITY_NONE, 
			stopbits     = serial.STOPBITS_ONE,
			bytesize     = serial.EIGHTBITS,
			writeTimeout = 10
		)

	except serial.SerialException as e:
		print("! Erreur lors de l'ouverture du port serie : " + str(e))
		
	if ser.isOpen():
		print("")
		print("  GO GO GO ...")

		try:
			ser.flushInput()	#flush input buffer, discarding all its contents
			ser.flushOutput()	#flush output buffer, aborting current output and discard all that is in buffer

			i 		  = 0

			joy_l_val = 0    	# la valeur du joystick de gauche
			joy_l_dir = '*'		# le signe  du joystick de gauche
			joy_l_str = '---'

			joy_r_val = 0    	# la valeur du joystick de droite
			joy_r_dir = '*'     # le signe  du joystick de droite
			joy_r_str = '---'


			toSend = False		# boolean indiquant qu qq chose est a envoyer
			trame_to_send = ''  # la chaine de caractere qui sera transmise


			# ...
			clock = pygame.time.Clock()

			# Variable qui continue la boucle si = True, stoppe si = False
			running = True

			#Boucle infinie
			while running:

				# rampe de vitesse positive 0 à 100 %
				# en dents de scie :

				# +100 % -     +   +   +   +
				#             /|  /|  /|  /|
				#            / | / | / | / |
				#           /  |/  |/  |/  |
				# -100 % - +   +   +   +   +

				for i in range(-100,100):
					
					##calcul roue droite
					if i < 0:
						joy_r_dir = '-'
					else:
						joy_r_dir = '+'
						
					joy_r_val = int(abs(i))
					joy_r_str = convStrOn3Digits(joy_r_val)

					##calcul roue gauche
					if i < 0:
						joy_l_dir = '+'
					else:
						joy_l_dir = '-'
						
					joy_l_val = int(abs(i))
					joy_l_str = convStrOn3Digits(joy_l_val)
					

					#concatane
					trame_to_send = "-v:" + str(joy_l_dir) + str(joy_l_str) + ',' + str(joy_r_dir) + str(joy_r_str) + ';'

					#send
					toSend = True
					

					if toSend is True:
						sendCmd(trame_to_send, ser)
						time.sleep(0.5)
						toSend = False


				clock.tick(20)
			
			# running = False	
			ser.close()

		except serial.SerialException as e:
			print(" ! Erreur de communication : " + str(e))


		
# Definitions
def scan_serial_ports():
	"""
	Description : A function that tries to list serial ports on most common platforms
	RESTRICTION : compatible seulement avec les ordinateurs de type WINDOWS
	"""
	# Scan for available ports
	available_ports = []
	for i in range(50):	#256
		try:
			s = serial.Serial(i)
			available_ports.append((i, s.portstr))
			s.close()
		except serial.SerialException:
			pass
	return available_ports


def sendCmd(cmd, ser):
	"""
	Description : Transmet une chaine de caracteres sur la liaison serie.

	Param(s) : cmd, la chaine de caracteres a envoyer
			   ser, ...

	Output : Nothing
	"""

	cmd += "\r\n"
	print(cmd)
	ser.write(cmd.encode('ascii'))


def convStrOn3Digits(value):
	"""
	Description : Converti une chaine de caracteres, representant un nb compris entre 0 et 100
				  en une chaine de caracteres, sur 3 digits

	Param(s) : value, la valeur a Convertir

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
