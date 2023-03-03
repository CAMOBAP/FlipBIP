#include "../flipbip39.h"
#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/elements.h>
#include <dolphin/dolphin.h>
#include "../helpers/flipbip39_haptic.h"
#include "../helpers/flipbip39_speaker.h"
#include "../helpers/flipbip39_led.h"
#include "../helpers/flipbip39_string.h"

#include <string.h>
#include "../helpers/printf.h"
#include "../crypto/bip32.h"
#include "../crypto/bip39.h"
// #include "../crypto/ecdsa.h"
#include "../crypto/curves.h"
#include "../crypto/memzero.h"

struct FlipBip39Scene1 {
    View* view;
    FlipBip39Scene1Callback callback;
    void* context;
};


typedef struct {
    int page;
    int strength;
    const char* seed1;
    const char* seed2;
    const char* seed3;
    const char* seed4;
    const char* seed5;
    const char* seed6;
    const char* mnemonic1;
    const char* mnemonic2;
    const char* mnemonic3;
    const char* mnemonic4;
    const char* mnemonic5;
    const char* mnemonic6;
} FlipBip39Scene1Model;

void flipbip39_scene_1_set_callback(
    FlipBip39Scene1* instance,
    FlipBip39Scene1Callback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void flipbip39_scene_1_draw(Canvas* canvas, FlipBip39Scene1Model* model) {
    //UNUSED(model);
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    //canvas_set_font(canvas, FontPrimary);
    //canvas_draw_str_aligned(canvas, 0, 10, AlignLeft, AlignTop, "This is Scene 1"); 
    
    canvas_set_font(canvas, FontSecondary);
    //canvas_draw_str_aligned(canvas, 1, 2, AlignLeft, AlignTop, model->strength == 128 ? "128-bit" : "256-bit");
    //canvas_draw_str_aligned(canvas, 1, 2, AlignLeft, AlignTop, model->seed);
    
    // Mnenomic
    if (model->page == 0) {
        canvas_draw_str_aligned(canvas, 1, 2, AlignLeft, AlignTop, model->mnemonic1);
        canvas_draw_str_aligned(canvas, 1, 12, AlignLeft, AlignTop, model->mnemonic2);
        canvas_draw_str_aligned(canvas, 1, 22, AlignLeft, AlignTop, model->mnemonic3);
        canvas_draw_str_aligned(canvas, 1, 32, AlignLeft, AlignTop, model->mnemonic4);
        canvas_draw_str_aligned(canvas, 1, 42, AlignLeft, AlignTop, model->mnemonic5);
        canvas_draw_str_aligned(canvas, 1, 52, AlignLeft, AlignTop, model->mnemonic6);
    }
    // Seed
    else if (model->page == 1) {
        canvas_draw_str_aligned(canvas, 1, 2, AlignLeft, AlignTop, model->seed1);
        canvas_draw_str_aligned(canvas, 1, 12, AlignLeft, AlignTop, model->seed2);
        canvas_draw_str_aligned(canvas, 1, 22, AlignLeft, AlignTop, model->seed3);
        canvas_draw_str_aligned(canvas, 1, 32, AlignLeft, AlignTop, model->seed4);
        canvas_draw_str_aligned(canvas, 1, 42, AlignLeft, AlignTop, model->seed5);
        canvas_draw_str_aligned(canvas, 1, 52, AlignLeft, AlignTop, model->seed6);
    }

    
}

static void flipbip39_scene_1_model_init(FlipBip39Scene1Model* const model, const int strength) {
    
    model->page = 0;

    // Generate a random mnemonic using trezor-crypto
    model->strength = strength;
    const char *mnemonic = mnemonic_generate(model->strength);

    // Delineate sections of the mnemonic every 4 words
    char *mnemo = malloc(strlen(mnemonic) + 1);
    strcpy(mnemo, mnemonic);
    int word = 0;
    for (size_t i = 0; i < strlen(mnemo); i++) {
        if (mnemo[i] == ' ') {
            word++;
            if (word % 4 == 0) {
                mnemo[i] = ',';
            }
        } 
    }

    // Split the mnemonic into parts
    char *mnemopart = flipbip39_strtok(mnemo, ",");
    int partnum = 0;
    while(mnemopart != NULL)
    {
        char *partptr = malloc(strlen(mnemopart) + 1);
        strcpy(partptr, mnemopart);
        partnum++;
        
        if (partnum == 1) model->mnemonic1 = partptr;
        if (partnum == 2) model->mnemonic2 = partptr;
        if (partnum == 3) model->mnemonic3 = partptr;
        if (partnum == 4) model->mnemonic4 = partptr;
        if (partnum == 5) model->mnemonic5 = partptr;
        if (partnum == 6) model->mnemonic6 = partptr;

        mnemopart = flipbip39_strtok(NULL, ",");
    }

    // Generate a BIP39 seed from the mnemonic
    uint8_t seedbytes[64];
    mnemonic_to_seed(mnemonic, "", seedbytes, 0);
    char *seed = malloc(64 * 2 + 1);
    
    // Convert the seed to a hex string
    for (size_t i = 0; i < 64; i++) {
        sprintf(seed + (i * 2), "%.2x", seedbytes[i]);
    }
    
    // Split the seed into parts
    for (size_t seedpartnum = 1; seedpartnum <= 6; seedpartnum++) {
        char *seedpartptr = malloc(22 + 1);
        strncpy(seedpartptr, seed + ((seedpartnum - 1) * 22), 22);
        
        if (seedpartnum == 1) model->seed1 = seedpartptr;
        if (seedpartnum == 2) model->seed2 = seedpartptr;
        if (seedpartnum == 3) model->seed3 = seedpartptr;
        if (seedpartnum == 4) model->seed4 = seedpartptr;
        if (seedpartnum == 5) model->seed5 = seedpartptr;
        if (seedpartnum == 6) model->seed6 = seedpartptr;
    }

    // WIP / TODO: Generate a BIP32 root key from the mnemonic

    // //bool root_set = false;
    HDNode *root = malloc(sizeof(HDNode));
    hdnode_from_seed(seedbytes, 64, SECP256K1_NAME, root);
    // //root_set = true;

    int account = 0;
    // constants for Bitcoin
    const uint32_t version_public = 0x0488b21e;
    const uint32_t version_private = 0x0488ade4;
    //const char addr_version = 0x00, wif_version = 0x80;
    const size_t buflen = 128;
    char buf[buflen + 1];
    //HDNode node;
    uint32_t fingerprint = 0;
    hdnode_serialize_private(root, fingerprint, version_private, buf, buflen); 
    char *xprvroot = malloc(22 + 1);
    strncpy(xprvroot, buf, 22);
    model->seed2 = xprvroot;
    // external chain
    for (int chain = 0; chain < 2; chain++) {
        //QTableWidget *list = chain == 0 ? ui->listAddress : ui->listChange;
        HDNode *node = root;
        fingerprint = hdnode_fingerprint(node);
        hdnode_private_ckd_prime(node, 44); // purpose
        fingerprint = hdnode_fingerprint(node);
        hdnode_private_ckd_prime(node, 0); // coin (bitcoin)
        fingerprint = hdnode_fingerprint(node);
        hdnode_private_ckd_prime(node, account); // account
        hdnode_serialize_private(node, fingerprint, version_private, buf, buflen); 
        char *xprv = malloc(22 + 1);
        strncpy(xprv, buf, 22);
        if (chain == 0) model->seed3 = xprv;
        if (chain == 1) model->seed4 = xprv;
        //QString xprv = QString(buf); ui->lineXprv->setText(xprv);
        hdnode_serialize_public(node, fingerprint, version_public, buf, buflen); 
        char *xpub = malloc(22 + 1);
        strncpy(xpub, buf, 22);
        if (chain == 0) model->seed5 = xpub;
        if (chain == 1) model->seed6 = xpub;
        //QString xpub = QString(buf); ui->lineXpub->setText(xpub);
        // hdnode_private_ckd(&node, chain); // external / internal
        // for (int i = 0; i < 10; i++) {
        //     HDNode node2 = node;
        //     hdnode_private_ckd(&node2, i);
        //     hdnode_fill_public_key(&node2);
        //     ecdsa_get_address(node2.public_key, addr_version, HASHER_SHA2_RIPEMD, HASHER_SHA2D, buf, buflen); 
        //     //QString address = QString(buf);
        //     ecdsa_get_wif(node2.private_key, wif_version, HASHER_SHA2D, buf, buflen); 
        //     //QString wif = QString(buf);
        //     // list->setItem(i, 0, new QTableWidgetItem(address));
        //     // list->setItem(i, 1, new QTableWidgetItem(wif));
        //     // list->setItem(i, 2, new QTableWidgetItem("0.0"));
        // }
    }

    // Clear the root node
    memzero(root, sizeof(HDNode));
    free(root);

    // Clear the mnemonic
    mnemonic_clear();
    memzero(mnemo, strlen(mnemo));
    free(mnemo);

    // Clear the seed
    memzero(seed, sizeof(seed));
    free(seed);

    // Clear the BIP39 cache
    bip39_cache_clear();
}

bool flipbip39_scene_1_input(InputEvent* event, void* context) {
    furi_assert(context); 
    FlipBip39Scene1* instance = context;
    if (event->type == InputTypeRelease) {
        switch(event->key) {
            case InputKeyBack:
                with_view_model(
                    instance->view,
                    FlipBip39Scene1Model * model,
                    {
                        UNUSED(model);
                        instance->callback(FlipBip39CustomEventScene1Back, instance->context);
                    },
                    true);
                break;
            case InputKeyLeft:
            case InputKeyRight:
            case InputKeyUp:
            case InputKeyDown:
            case InputKeyOk:
                with_view_model(
                    instance->view,
                    FlipBip39Scene1Model* model,
                    {
                        //UNUSED(model);
                        model->page = (model->page + 1) % 2;
                    },
                    true);
                break;
            case InputKeyMAX:
                break;
        }
    }
    return true;
}

void flipbip39_scene_1_exit(void* context) {
    furi_assert(context);
    FlipBip39Scene1* instance = (FlipBip39Scene1*)context;

    with_view_model(
        instance->view,
        FlipBip39Scene1Model * model,
        {
            // Clear the mnemonic from memory
            model->page = 0;
            model->strength = 0;
            memzero((void*)model->seed1, strlen(model->seed1));
            memzero((void*)model->seed2, strlen(model->seed2));
            memzero((void*)model->seed3, strlen(model->seed3));
            memzero((void*)model->seed4, strlen(model->seed4));
            memzero((void*)model->seed5, strlen(model->seed5));
            memzero((void*)model->seed6, strlen(model->seed6));
            memzero((void*)model->mnemonic1, strlen(model->mnemonic1));
            memzero((void*)model->mnemonic2, strlen(model->mnemonic2));
            memzero((void*)model->mnemonic3, strlen(model->mnemonic3));
            memzero((void*)model->mnemonic4, strlen(model->mnemonic4));
            memzero((void*)model->mnemonic5, strlen(model->mnemonic5));
            memzero((void*)model->mnemonic6, strlen(model->mnemonic6));
        },
        true
    );
}

void flipbip39_scene_1_enter(void* context) {
    furi_assert(context);
    FlipBip39Scene1* instance = (FlipBip39Scene1*)context;

    FlipBip39* app = instance->context;
    int strength_setting = app->bip39_strength;
    int strength = 256;
    if (strength_setting == 0) strength = 128;
    else if (strength_setting == 1) strength = 192;

    flipbip39_play_happy_bump(app);
    flipbip39_led_set_rgb(app, 255, 0, 0);

    with_view_model(
        instance->view,
        FlipBip39Scene1Model * model,
        {
            flipbip39_scene_1_model_init(model, strength);
        },
        true
    );
}

FlipBip39Scene1* flipbip39_scene_1_alloc() {
    FlipBip39Scene1* instance = malloc(sizeof(FlipBip39Scene1));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(FlipBip39Scene1Model));
    view_set_context(instance->view, instance); // furi_assert crashes in events without this
    view_set_draw_callback(instance->view, (ViewDrawCallback)flipbip39_scene_1_draw);
    view_set_input_callback(instance->view, flipbip39_scene_1_input);
    view_set_enter_callback(instance->view, flipbip39_scene_1_enter);
    view_set_exit_callback(instance->view, flipbip39_scene_1_exit);

    // FlipBip39* app = instance->context;
    // int strength_setting = app->bip39_strength;
    // int strength = 256;
    // if (strength_setting == 0) strength = 128;
    // else if (strength_setting == 1) strength = 192;

    // with_view_model(
    //     instance->view,
    //     FlipBip39Scene1Model * model,
    //     {
    //         flipbip39_scene_1_model_init(model, strength);
    //     },
    //     true
    // );
    
    return instance;
}

void flipbip39_scene_1_free(FlipBip39Scene1* instance) {
    furi_assert(instance);

    with_view_model(
        instance->view,
        FlipBip39Scene1Model * model,
        {
            //UNUSED(model);
            free((void*)model->seed1);
            free((void*)model->seed2);
            free((void*)model->seed3);
            free((void*)model->seed4);
            free((void*)model->seed5);
            free((void*)model->seed6);
            free((void*)model->mnemonic1);
            free((void*)model->mnemonic2);
            free((void*)model->mnemonic3);
            free((void*)model->mnemonic4);
            free((void*)model->mnemonic5);
            free((void*)model->mnemonic6);
        },
        true);
    view_free(instance->view);
    free(instance);
}

View* flipbip39_scene_1_get_view(FlipBip39Scene1* instance) {
    furi_assert(instance);
    return instance->view;
}

