#ifndef POLARSSL_MD_H
#define POLARSSL_MD_H

#include <stdlib.h>

#define POLARSSL_ERR_MD_FEATURE_UNAVAILABLE                -0x5080  /**< The selected feature is not available. */
#define POLARSSL_ERR_MD_BAD_INPUT_DATA                     -0x5100  /**< Bad input parameters to function. */
#define POLARSSL_ERR_MD_ALLOC_FAILED                       -0x5180  /**< Failed to allocate memory. */
#define POLARSSL_ERR_MD_FILE_IO_ERROR                      -0x5200  /**< Opening or reading of file failed. */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    POLARSSL_MD_NONE=0,
    POLARSSL_MD_SHA224,
    POLARSSL_MD_SHA256,
    POLARSSL_MD_SHA384,
    POLARSSL_MD_SHA512,
} md_type_t;

#if defined(POLARSSL_SHA512_C)
#define POLARSSL_MD_MAX_SIZE         64  /* longest known is SHA512 */
#else
#define POLARSSL_MD_MAX_SIZE         32  /* longest known is SHA256 or less */
#endif

/**
 * Message digest information. Allows message digest functions to be called
 * in a generic way.
 */
typedef struct
{
    /** Digest identifier */
    md_type_t type;

    /** Name of the message digest */
    const char * name;

    /** Output length of the digest function */
    int size;

    /** Digest initialisation function */
    void (*starts_func)( void *ctx );

    /** Digest update function */
    void (*update_func)( void *ctx, const uint8_t *input, size_t ilen );

    /** Digest finalisation function */
    void (*finish_func)( void *ctx, uint8_t *output );

    /** Generic digest function */
    void (*digest_func)( const uint8_t *input, size_t ilen, uint8_t *output );

    /** HMAC Initialisation function */
    void (*hmac_starts_func)( void *ctx, const uint8_t *key, size_t keylen );

    /** HMAC update function */
    void (*hmac_update_func)( void *ctx, const uint8_t *input, size_t ilen );

    /** HMAC finalisation function */
    void (*hmac_finish_func)( void *ctx, uint8_t *output);

    /** HMAC context reset function */
    void (*hmac_reset_func)( void *ctx );

    /** Generic HMAC function */
    void (*hmac_func)( const uint8_t *key, size_t keylen, const uint8_t *input, size_t ilen, uint8_t *output );

    /** Allocate a new context */
    void * (*ctx_alloc_func)( void );

    /** Free the given context */
    void (*ctx_free_func)( void *ctx );

    /** Internal use only */
    void (*process_func)( void *ctx, const uint8_t *input );
} md_info_t;

/**
 * Generic message digest context.
 */
typedef struct {
    /** Information about the associated message digest */
    const md_info_t *md_info;

    /** Digest-specific context */
    void *md_ctx;
} md_context_t;

#define MD_CONTEXT_T_INIT { \
    NULL, /* md_info */ \
    NULL, /* md_ctx */ \
}

/**
 * \brief Returns the list of digests supported by the generic digest module.
 *
 * \return          a statically allocated array of digests, the last entry
 *                  is 0.
 */
const int *md_list( void );

/**
 * \brief           Returns the message digest information associated with the
 *                  given digest name.
 *
 * \param md_name   Name of the digest to search for.
 *
 * \return          The message digest information associated with md_name or
 *                  NULL if not found.
 */
const md_info_t *md_info_from_string( const char *md_name );

/**
 * \brief           Returns the message digest information associated with the
 *                  given digest type.
 *
 * \param md_type   type of digest to search for.
 *
 * \return          The message digest information associated with md_type or
 *                  NULL if not found.
 */
const md_info_t *md_info_from_type( md_type_t md_type );

/**
 * \brief          Initialises and fills the message digest context structure
 *                 with the appropriate values.
 *
 * \param ctx      context to initialise. May not be NULL. The
 *                 digest-specific context (ctx->md_ctx) must be NULL. It will
 *                 be allocated, and must be freed using md_free_ctx() later.
 * \param md_info  message digest to use.
 *
 * \returns        \c 0 on success, \c POLARSSL_ERR_MD_BAD_INPUT_DATA on
 *                 parameter failure, \c POLARSSL_ERR_MD_ALLOC_FAILED if
 *                 allocation of the digest-specific context failed.
 */
int md_init_ctx( md_context_t *ctx, const md_info_t *md_info );

/**
 * \brief          Free the message-specific context of ctx. Freeing ctx itself
 *                 remains the responsibility of the caller.
 *
 * \param ctx      Free the message-specific context
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_free_ctx( md_context_t *ctx );

/**
 * \brief           Returns the size of the message digest output.
 *
 * \param md_info   message digest info
 *
 * \return          size of the message digest output.
 */
static uint8_t md_get_size( const md_info_t *md_info )
{
    if( md_info == NULL )
        return( 0 );

    return md_info->size;
}

/**
 * \brief           Returns the type of the message digest output.
 *
 * \param md_info   message digest info
 *
 * \return          type of the message digest output.
 */
static md_type_t md_get_type( const md_info_t *md_info )
{
    if( md_info == NULL )
        return( POLARSSL_MD_NONE );

    return md_info->type;
}

/**
 * \brief           Returns the name of the message digest output.
 *
 * \param md_info   message digest info
 *
 * \return          name of the message digest output.
 */
static const char *md_get_name( const md_info_t *md_info )
{
    if( md_info == NULL )
        return( NULL );

    return md_info->name;
}

/**
 * \brief          Set-up the given context for a new message digest
 *
 * \param ctx      generic message digest context.
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_starts( md_context_t *ctx );

/**
 * \brief          Generic message digest process buffer
 *
 * \param ctx      Generic message digest context
 * \param input    buffer holding the  datal
 * \param ilen     length of the input data
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_update( md_context_t *ctx, const uint8_t *input, size_t ilen );

/**
 * \brief          Generic message digest final digest
 *
 * \param ctx      Generic message digest context
 * \param output   Generic message digest checksum result
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_finish( md_context_t *ctx, uint8_t *output );

/**
 * \brief          Output = message_digest( input buffer )
 *
 * \param md_info  message digest info
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   Generic message digest checksum result
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md( const md_info_t *md_info, const uint8_t *input, size_t ilen,
        uint8_t *output );

/**
 * \brief          Output = message_digest( file contents )
 *
 * \param md_info  message digest info
 * \param path     input file name
 * \param output   generic message digest checksum result
 *
 * \return         0 if successful, POLARSSL_ERR_MD_FILE_OPEN_FAILED if fopen
 *                 failed, POLARSSL_ERR_MD_FILE_READ_FAILED if fread failed,
 *                 POLARSSL_ERR_MD_BAD_INPUT_DATA if md_info was NULL.
 */
//int md_file( const md_info_t *md_info, const char *path, uint8_t *output );

/**
 * \brief          Generic HMAC context setup
 *
 * \param ctx      HMAC context to be initialized
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_hmac_starts( md_context_t *ctx, const uint8_t *key,
                    size_t keylen );

/**
 * \brief          Generic HMAC process buffer
 *
 * \param ctx      HMAC context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_hmac_update( md_context_t *ctx, const uint8_t *input,
                    size_t ilen );

/**
 * \brief          Generic HMAC final digest
 *
 * \param ctx      HMAC context
 * \param output   Generic HMAC checksum result
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_hmac_finish( md_context_t *ctx, uint8_t *output);

/**
 * \brief          Generic HMAC context reset
 *
 * \param ctx      HMAC context to be reset
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_hmac_reset( md_context_t *ctx );

/**
 * \brief          Output = Generic_HMAC( hmac key, input buffer )
 *
 * \param md_info  message digest info
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   Generic HMAC-result
 *
 * \returns        0 on success, POLARSSL_ERR_MD_BAD_INPUT_DATA if parameter
 *                 verification fails.
 */
int md_hmac( const md_info_t *md_info, const uint8_t *key, size_t keylen,
                const uint8_t *input, size_t ilen,
                uint8_t *output );

/* Internal use */
int md_process( md_context_t *ctx, const uint8_t *data );

#ifdef __cplusplus
}
#endif

#endif /* POLARSSL_MD_H */
