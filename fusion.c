#include <gtk/gtk.h>


static void on_button_clicked(GtkWidget *widget, gpointer user_data) {
    g_print("Button Clicked\n");
}

static void load_css (void){
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;

const gchar *css_style_file = "style.css";
GFile *css_fp =g_file_new_for_path (css_style_file);
GError *error = 0;

provider = gtk_css_provider_new();
display = gdk_display_get_default();
screen = gdk_display_get_default_screen(display);

gtk_style_contexte_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTKSTYLE_PROVIDER_PRIORITY_APPLICATION);
gtk_css_provider_load_from_file(provider, css_fp, &error);

g_object_unref(provider);
}


// Fonction pour changer la couleur du bouton
/*void change_button_color(GtkWidget *button, const char *color_str) {
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_class(context, "custom-button");

    gchar *css = g_strdup_printf(".custom-button { background-color: %s; }", color_str);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_free(css);
}*/

static void on_activate(GtkApplication *app, gpointer user_data) {

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 1354, 650);

    // Créer une grille
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Ajouter le champ de saisie
    GtkWidget *entry = gtk_entry_new();
    // Ajuster la taille du champ de saisie
    gtk_widget_set_size_request(entry, 120, 40);
    // Modifier la position du champ de saisie dans la grille (en bas)
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);

    // Ajouter le bouton "SIZE"
    GtkWidget *button = gtk_button_new_with_label("SIZE");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Ajuster la taille du bouton
    gtk_widget_set_size_request(button, 100, 90);

    // Modifier la couleur de fond du bouton (en bleu ciel)
   // change_button_color(button, "rgb(173, 216, 230)");

    // Modifier la position du bouton dans la grille (en bas)
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

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
