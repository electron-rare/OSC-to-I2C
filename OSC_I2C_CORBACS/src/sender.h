void servo_send(int servo_select)
{
    if (osc_value_old[servo_select] != osc_value[servo_select]) // mise à jour si changement de valeur
    {
        osc_value_old[servo_select] = osc_value[servo_select]; // sauvegarde de la valeur
        int servo_value = map(osc_value[servo_select], min_osc_value, max_osc_value, -90, 90);        // conversion de l'osc en valeur d'angle
        int pulse = map(servo_value, -90, 90, servomoins90[servo_select], servoplus90[servo_select]); // adaptation de l'angle à la plage du servo

        // selection de la carte de sortie
        if (servo_select < nb_out_board) // 1 à 16
        {
            board1.setPWM(servomoteur[servo_select], 0, pulse);
        }
        else if (servo_select < nb_out_board * 2) // 17 à 32
        {
            board2.setPWM(servomoteur[servo_select - nb_out_board], 0, pulse);
        }
        else if (servo_select < nb_out_board * 3) // 33 à 48
        {
            board3.setPWM(servomoteur[servo_select - nb_out_board * 2], 0, pulse);
        }
#ifdef DEBUG
        Serial.print("moteur : ");
        Serial.println(servo_select);
        Serial.print("valeur osc : ");
        Serial.println(osc_value[servo_select]);
        Serial.print("valeur pulse : ");
        Serial.println(pulse);
#endif
    }
}
