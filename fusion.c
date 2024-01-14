#include <gtk/gtk.h>

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 1354, 650);

    // Créer une boîte
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Modifier la couleur de fond de la fenêtre en vert menthe foncé
    GdkRGBA color;
    gdk_rgba_parse(&color, "rgb(1, 65, 52)");  // Ces valeurs correspondent à un vert menthe plus foncé
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);

    // Afficher la fenêtre
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.example.triparfusion", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
