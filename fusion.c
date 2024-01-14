#include <gtk/gtk.h>

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 1354, 650);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

  //  GtkWidget *button1 = gtk_button_new_with_label("Exécuter l'algorithme");
  //  gtk_box_pack_start(GTK_BOX(box), button1, TRUE, TRUE, 0);

   // GtkWidget *button2 = gtk_button_new_with_label("Quitter");
    //gtk_box_pack_start(GTK_BOX(box), button2, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.example.triparfusion", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
