from transformers import pipeline
def translate(source_lang, target_lang, text):
    translation = f"translation_{source_lang}_to_{target_lang}"
    model_name = "facebook/m2m100_418M"
    translator = pipeline(translation, model=model_name)
    result = translator(text, src_lang=source_lang, tgt_lang=target_lang)
    return result