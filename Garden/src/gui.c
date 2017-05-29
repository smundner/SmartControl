#include "gui.h"
#include <gtk/gtk.h>

void ende(){
	gtk_main_quit();
	}
int on[8];
int off[8];

void relay_button(GtkButton *button, gpointer user_data){
	int relay = *((int *)user_data)/10;
	int command = *((int *)user_data)-(relay*10);
	g_print("Relay %i %i\n",relay,command);
	}
//Erstellen vom Einstellungs Tab
GtkWidget *einstellungen_create(){
	GtkWidget *einstellungen;
	einstellungen = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
	
	GtkWidget *button;
	button = gtk_button_new_with_label("exit");
	g_signal_connect(button,"clicked",G_CALLBACK(ende),NULL);
	gtk_container_add(GTK_CONTAINER(einstellungen),button);
	
	return einstellungen;
	}
//Test Tab
GtkWidget *test_create(){
	GtkWidget *test_box;
	test_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
		
	GtkWidget *label;
	label = gtk_label_new("test");
	gtk_container_add(GTK_CONTAINER(test_box),label);
	//Row
	
	for(int i = 0;i<8;i++){
		GtkWidget *row_label, *row_button_on, *row_button_off, *row_box;
		row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
		gchar labelText[10];
		sprintf(labelText,"Relay %i",i+1);
		row_label = gtk_label_new(labelText);
		gtk_container_add(GTK_CONTAINER(row_box),row_label);
	
		row_button_on = gtk_button_new_with_label("On");
		
		on[i]=(10 * (i+1)) +1;
		g_signal_connect(row_button_on,"clicked",G_CALLBACK(relay_button),&on[i]);
		gtk_container_add(GTK_CONTAINER(row_box),row_button_on);
		row_button_off = gtk_button_new_with_label("Off");
		
		off[i]=(10*(i+1));
		g_signal_connect(row_button_off,"clicked",G_CALLBACK(relay_button),&off[i]);
		gtk_container_add(GTK_CONTAINER(row_box),row_button_off);
		gtk_container_add(GTK_CONTAINER(test_box),row_box);
	}
	return test_box;
	}
//Program Tab
GtkWidget *program_create(){
	GtkWidget *program;
	program = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
	
	GtkWidget *title;
	title = gtk_label_new("Programm");
	gtk_container_add(GTK_CONTAINER(program),title);
	
	GtkWidget *hbox, *combo_box_programm, *button_add_programm, *button_remove_programm;
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
	combo_box_programm = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box_programm),NULL, "Programm 1");
	gtk_container_add(GTK_CONTAINER(hbox),combo_box_programm);
	
	button_add_programm = gtk_button_new_with_label("Add");
	gtk_container_add(GTK_CONTAINER(hbox),button_add_programm);
	button_remove_programm = gtk_button_new_with_label("Remove");
	gtk_container_add(GTK_CONTAINER(hbox),button_remove_programm);
	
	GtkWidget *spalt;
	spalt = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(hbox),spalt);
	
	GtkWidget *combo_box_sperre, *button_add_sperre, *button_remove_sperre;
	combo_box_sperre = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box_sperre),NULL, "Regensensor");
	gtk_container_add(GTK_CONTAINER(hbox),combo_box_sperre);
	
	button_add_sperre = gtk_button_new_with_label("Add");
	gtk_container_add(GTK_CONTAINER(hbox),button_add_sperre);
	button_remove_sperre = gtk_button_new_with_label("Remove");
	gtk_container_add(GTK_CONTAINER(hbox),button_remove_sperre);
	
	gtk_container_add(GTK_CONTAINER(program),hbox);
	return program;
	}
//Starte Gui
void start_gui(int argc, char *argv[]){
	gtk_init(&argc, &argv);
    
    //Window
    GtkWidget *w;
    w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(w), "Smart Garden");
    gtk_window_set_default_size(GTK_WINDOW(w), 800, 600);
    gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(w),25);
	g_signal_connect(w,"destroy",G_CALLBACK(ende),NULL);
	
	//Tap
	GtkWidget *tap_stack1;
	tap_stack1 = gtk_stack_new();

	gtk_stack_add_titled(GTK_STACK(tap_stack1),test_create(),"test","test");
	gtk_stack_add_titled(GTK_STACK(tap_stack1),program_create(),"Programm","Programm");
	gtk_stack_add_titled(GTK_STACK(tap_stack1),einstellungen_create(),"Einstellungen","Einstellungen");
	
	
	//Tap switcher
	GtkWidget *tab_switcher;
	tab_switcher = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(tab_switcher),GTK_STACK(tap_stack1));
	
	GtkWidget *header;
	header = gtk_header_bar_new();
	gtk_header_bar_set_title(GTK_HEADER_BAR(header),"Smart Garden");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header),TRUE);
	gtk_container_add(GTK_CONTAINER(header),tab_switcher);
	
	GtkWidget *vbox;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
	gtk_container_add(GTK_CONTAINER(vbox),tap_stack1);
	gtk_window_set_titlebar(GTK_WINDOW(w),header);
	gtk_container_add(GTK_CONTAINER(w),vbox);
    gtk_widget_show_all(w);
    gtk_main();
}
