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

// Fonction pour créer le modèle du GtkTreeView
static GtkTreeModel *create_and_fill_model(UserData *user_data_struct) {
    GtkListStore *store;
    GtkTreeIter iter;

    // Créer le modèle
    store = gtk_list_store_new(1, G_TYPE_INT);

    // Remplir le modèle avec les entiers du vecteur
    for (int i = 0; i < user_data_struct->vector_size; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, user_data_struct->integer_array[i], -1);
    }

    return GTK_TREE_MODEL(store);
}



// Callback pour le clic sur le bouton "SIZE"
static void on_size_button_clicked(GtkWidget *button, gpointer user_data) {
    // Récupérer la valeur de l'entier depuis le GtkSpinButton
    GtkSpinButton *spin_button = GTK_SPIN_BUTTON(user_data);
    int vector_size = gtk_spin_button_get_value_as_int(spin_button);

    // Récupérer la structure UserData depuis le bouton
    UserData *user_data_struct = (UserData *)g_object_get_data(G_OBJECT(button), "user_data");

    // Allouer de la mémoire pour le vecteur
    if (user_data_struct->integer_array != NULL) {
        g_free(user_data_struct->integer_array);
    }
    user_data_struct->integer_array = g_malloc(vector_size * sizeof(int));
    user_data_struct->vector_size = vector_size;

    // Récupérer la liste d'entiers depuis le GtkEntry
    GtkWidget *entry_list = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "entry_list"));
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry_list));

    // Remplir le vecteur avec les valeurs saisies
    gchar **tokens = g_strsplit(entry_text, ",", -1);
    for (int i = 0; i < vector_size; i++) {
        user_data_struct->integer_array[i] = atoi(tokens[i]);
    }
    g_strfreev(tokens);

    // Mettre à jour le modèle du GtkTreeView
    GtkTreeView *treeview = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(button), "treeview"));
    gtk_tree_view_set_model(treeview, create_and_fill_model(user_data_struct));
}


static void on_sort_button_clicked(GtkWidget *button, gpointer user_data) {
    // Récupérer la structure UserData depuis le bouton "SIZE"
    UserData *user_data_struct = (UserData *)g_object_get_data(G_OBJECT(button), "user_data");

    // Récupérer la fenêtre parente
    GtkWindow *parent_window = GTK_WINDOW(gtk_widget_get_toplevel(button));

    // Appliquer le tri par fusion
    merge_sort(user_data_struct, 0, user_data_struct->vector_size - 1, parent_window);

    // Mettre à jour le modèle du GtkTreeView avec le tableau trié
    GtkTreeView *treeview = GTK_TREE_VIEW(g_object_get_data(G_OBJECT(button), "treeview"));
    gtk_tree_view_set_model(treeview, create_and_fill_model(user_data_struct));
}



static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri par fusion");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 650);

    // Créer une grille
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

    // Créer la structure UserData
    UserData *user_data_struct = g_new(UserData, 1);
    user_data_struct->integer_array = NULL;
    user_data_struct->vector_size = 0;

    // Ajouter la structure UserData comme data pour le bouton
    g_object_set_data(G_OBJECT(button), "user_data", user_data_struct);

    // Créer le modèle du GtkTreeView
    GtkTreeModel *model = create_and_fill_model(user_data_struct);

    // Créer le GtkTreeView
    GtkWidget *treeview = gtk_tree_view_new_with_model(model);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
    g_object_set_data(G_OBJECT(button), "treeview", treeview);

    // Créer une colonne pour afficher les entiers
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Entiers", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Modifier la position du bloc en bas à côté du bord de la fenêtre
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), treeview, 0, 2, 1, 1);

    // Modifier la couleur de fond de la fenêtre en vert menthe foncé
    GdkRGBA color;
    gdk_rgba_parse(&color, "rgb(1, 65, 52)");  // Ces valeurs correspondent à un vert menthe plus foncé
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);


        // Ajouter le bouton "TRIER"
    GtkWidget *sort_button = gtk_button_new_with_label("TRIER");
    gtk_widget_set_size_request(sort_button, 100, 90);
    change_button_color(sort_button, "rgb(173, 216, 230)");
    g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), sort_button, 2, 0, 1, 1);



    // Afficher la fenêtre
    gtk_widget_show_all(window);
}

// Fonction pour libérer la mémoire allouée pour le vecteur
static void on_destroy(GtkWidget *widget, gpointer user_data) {
    UserData *user_data_struct = (UserData *)g_object_get_data(G_OBJECT(widget), "user_data");
    if (user_data_struct->integer_array != NULL) {
        g_free(user_data_struct->integer_array);
    }
    g_free(user_data_struct);
}


// Fonction pour effectuer le tri par fusion
void merge_sort(UserData *user_data, int left, int right, GtkWindow *parent_window) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Trier les deux moitiés
        merge_sort(user_data, left, mid, parent_window);
        merge_sort(user_data, mid + 1, right, parent_window);

        // Fusionner les deux moitiés triées
        merge(user_data, left, mid, right);

        // Afficher le tableau à chaque étape du tri
        gchar *step_text = g_strdup_printf("Étape du tri : ");
        for (int i = 0; i < user_data->vector_size; i++) {
            gchar *int_text = g_strdup_printf("%d ", user_data->integer_array[i]);
            step_text = g_strconcat(step_text, int_text, NULL);
            g_free(int_text);
        }

        // Afficher le texte dans une fenêtre de dialogue
        GtkWidget *dialog = gtk_message_dialog_new(parent_window,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"%s",step_text);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        g_free(step_text);
    }
}
// Fonction pour fusionner deux sous-tableaux de user_data->integer_array[]
void merge(UserData *user_data, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Créer des tableaux temporaires
    int L[n1], R[n2];

    // Copier les données vers les tableaux temporaires L[] et R[]
    for (i = 0; i < n1; i++)
        L[i] = user_data->integer_array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = user_data->integer_array[mid + 1 + j];

    // Fusionner les tableaux temporaires L[] et R[] dans user_data->integer_array[left..right]
    i = 0; // Index initial du premier sous-tableau
    j = 0; // Index initial du second sous-tableau
    k = left; // Index initial du tableau fusionné
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            user_data->integer_array[k] = L[i];
            i++;
        } else {
            user_data->integer_array[k] = R[j];
            j++;
        }
        k++;
    }

    // Copier les éléments restants de L[], s'il y en a
    while (i < n1) {
        user_data->integer_array[k] = L[i];
        i++;
        k++;
    }

    // Copier les éléments restants de R[], s'il y en a
    while (j < n2) {
        user_data->integer_array[k] = R[j];
        j++;
        k++;
    }
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.example.triparfusion", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Connecter la fonction de libération de mémoire à l'événement de destruction de la fenêtre
    g_signal_connect(app, "shutdown", G_CALLBACK(on_destroy), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Maintenant, après la fermeture de la fenêtre, triez et affichez le vecteur directement sur la console
    if (status == 0) {
        // Créer une instance de UserData
        UserData user_data;
        user_data.integer_array = g_malloc(5 * sizeof(int));  // Remplacez 5 par la taille de votre vecteur
        user_data.vector_size = 5;  // Remplacez 5 par la taille de votre vecteur

        // Remplir le vecteur avec des valeurs
        for (int i = 0; i < user_data.vector_size; i++) {
            user_data.integer_array[i] = g_random_int();  // Vous pouvez remplacer g_random_int() par vos propres valeurs
        }

        // Afficher le vecteur avant le tri
        printf("Vecteur avant le tri : ");
        for (int i = 0; i < user_data.vector_size; i++) {
            printf("%d ", user_data.integer_array[i]);
        }
        printf("\n");

        // Appliquer le tri par fusion
        merge_sort(&user_data, 0, user_data.vector_size - 1, NULL);

        // Afficher le vecteur après le tri
        printf("Vecteur après le tri : ");
        for (int i = 0; i < user_data.vector_size; i++) {
            printf("%d ", user_data.integer_array[i]);
        }
        printf("\n");

        // Libérer la mémoire allouée pour le vecteur
        g_free(user_data.integer_array);
    }

    g_object_unref(app);

    return status;
}
