
# write main method
from barttransformer import translate
if __name__ == '__main__':
    # Example usage
    source_lang = "en"
    target_lang = "zh"
    text = "Hello, how are you?"
    translation = translate(source_lang, target_lang, text)
    print(translation)