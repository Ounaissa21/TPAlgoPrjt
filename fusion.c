#include <gtk/gtk.h>

typedef struct {
    int *integer_array;  // Vecteur pour stocker les entiers
    int vector_size;     // Taille du vecteur
} UserData;

// Fonction pour changer la couleur du bouton
void change_button_color(GtkWidget *button, const char *color_str) {
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_class(context, "custom-button");

    gchar *css = g_strdup_printf(".custom-button { background-color: %s; }", color_str);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_free(css);
}

// Fonction pour cr�er le mod�le du GtkTreeView
static GtkTreeModel *create_and_fill_model(UserData *user_data_struct) {
    GtkListStore *store;
    GtkTreeIter iter;

    // Cr�er le mod�le
    store = gtk_list_store_new(1, G_TYPE_INT);

    // Remplir le mod�le avec les entiers du vecteur
    for (int i = 0; i < user_data_struct->vector_size; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, user_data_struct->integer_array[i], -1);
    }

    return GTK_TREE_MODEL(store);
}

// Callback pour le clic sur le bouton "SIZE"
static void on_size_button_clicked(GtkWidget *button, gpointer user_data) {
    // R�cup�rer la valeur de l'entier depuis le GtkSpinButton
    GtkSpinButton *spin_button = GTK_SPIN_BUTTON(user_data);
    int vector_size = gtk_spin_button_get_value_as_int(spin_button);

    // R�cup�rer la structure UserData depuis le bouton
    UserData *user_data_struct = (UserData *)g_object_get_data(G_OBJECT(button), "user_data");

    // Allouer de la m�moire pour le vecteur
    if (user_data_struct->integer_array != NULL) {
        g_free(user_data_struct->integer_array);
    }
    user_data_struct->integer_array = g_malloc(vector_size * sizeof(int));
    user_data_struct->vector_size = vector_size;

    // R�cup�rer la liste d'entiers depuis le GtkEntry
    GtkWidget *entry_list = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "entry_list"));
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry_list));

    // Remplir le vecteur avec les valeurs saisies
    gchar **tokens = g_strsplit(entry_text, ",", -1);
    for (int i = 0; i < vector_size; i++) {
        user_data_struct->integer_array[i] = atoi(tokens[i]);
    }
    g_strfreev(tokens);

    // Mettre � jour le mod�le du GtkTreeView
    GtkTreeView *treeview = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(button), "treeview"));
    gtk_tree_view_set_model(treeview, create_and_fill_model(user_data_struct));
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 1354, 650);

    // Cr�er une grille
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Ajouter le champ de saisie pour un entier avec GtkSpinButton
    GtkWidget *spin_button = gtk_spin_button_new_with_range(0, G_MAXINT, 1);
    gtk_widget_set_size_request(spin_button, 120, 40);
    gtk_grid_attach(GTK_GRID(grid), spin_button, 0, 0, 1, 1);

    // Ajouter le champ de saisie pour la liste d'entiers avec GtkEntry
    GtkWidget *entry_list = gtk_entry_new();
    gtk_widget_set_size_request(entry_list, 200, 40);
    gtk_grid_attach(GTK_GRID(grid), entry_list, 1, 0, 1, 1);

    // Ajouter le bouton "SIZE"
    GtkWidget *button = gtk_button_new_with_label("CREER");
    // Ajuster la taille du bouton
    gtk_widget_set_size_request(button, 100, 90);
    // Modifier la couleur de fond du bouton (en bleu ciel)
    change_button_color(button, "rgb(173, 216, 230)");
    // Connecter la fonction de callback au clic sur le bouton
    g_signal_connect(button, "clicked", G_CALLBACK(on_size_button_clicked), spin_button);

    // Ajouter le champ de saisie pour la liste d'entiers comme data pour la fonction de callback
    g_object_set_data(G_OBJECT(button), "entry_list", entry_list);

    // Cr�er la structure UserData
    UserData *user_data_struct = g_new(UserData, 1);
    user_data_struct->integer_array = NULL;
    user_data_struct->vector_size = 0;

    // Ajouter la structure UserData comme data pour le bouton
    g_object_set_data(G_OBJECT(button), "user_data", user_data_struct);

    // Cr�er le mod�le du GtkTreeView
    GtkTreeModel *model = create_and_fill_model(user_data_struct);

    // Cr�er le GtkTreeView
    GtkWidget *treeview = gtk_tree_view_new_with_model(model);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
    g_object_set_data(G_OBJECT(button), "treeview", treeview);

    // Cr�er une colonne pour afficher les entiers
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Entiers", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Modifier la position du bloc en bas � c�t� du bord de la fen�tre
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), treeview, 0, 2, 2, 1);

    // Modifier la couleur de fond de la fen�tre en vert menthe fonc�
    GdkRGBA color;
    gdk_rgba_parse(&color, "rgb(190, 238, 219)");  // Ces valeurs correspondent � un vert menthe plus fonc�
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);

    // Afficher la fen�tre
    gtk_widget_show_all(window);
}

// Fonction pour lib�rer la m�moire allou�e pour le vecteur
static void on_destroy(GtkWidget *widget, gpointer user_data) {
    UserData *user_data_struct = (UserData *)g_object_get_data(G_OBJECT(widget), "user_data");
    if (user_data_struct->integer_array != NULL) {
        g_free(user_data_struct->integer_array);
    }
    g_free(user_data_struct);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.example.triparfusion", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Connecter la fonction de lib�ration de m�moire � l'�v�nement de destruction de la fen�tre
    g_signal_connect(app, "shutdown", G_CALLBACK(on_destroy), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
